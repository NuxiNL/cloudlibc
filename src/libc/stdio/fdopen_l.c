// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/overflow.h>
#include <common/stdio.h>

#include <sys/stat.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Operations for descriptors that can be seeked (i.e., regular files).
//
// For regular files we only need a single buffer that sometimes acts as
// a read buffer and sometimes as a write buffer. We need to ensure that
// we flush data before performing a read or seek.
//
// We can use pread() and pwrite() to ensure that we do not clobber the
// file descriptor's offset. This makes it possible to have multiple
// FILE objects pointing to the same file descriptor.

static bool file_drain(FILE *file) __requires_exclusive(*file) {
  // Offset after draining.
  off_t offset = ftello_physical(file);
  bool refetch_offset = false;

  // Check whether there is data that still needs to be written to disk.
  while (file->file.written < file->writebuf) {
    size_t buflen = file->writebuf - file->file.written;
    ssize_t ret;
    if (file->file.append) {
      // File is opened for append. Call write().
      ret = write(file->fd, file->file.written, buflen);
      refetch_offset = true;
    } else {
      // File is not opened for append. We can use pwrite().
      ret = pwrite(file->fd, file->file.written, buflen, offset - buflen);
    }
    if (ret < 0) {
      file->flags |= F_ERROR;
      return false;
    }
    file->file.written += ret;
  }

  // If we've done a write() that has been opened with O_APPEND, refetch
  // the current offset within the file. This makes the offset go back
  // in sync if there are multiple writers to the same file.
  if (refetch_offset) {
    off_t new_offset = lseek(file->fd, 0, SEEK_CUR);
    if (new_offset >= 0)
      offset = new_offset;
  }

  // Discard both the read and write buffers.
  file->offset = offset;
  file->readbuf = NULL;
  file->readbuflen = 0;
  file->writebuf = file->file.written = NULL;
  file->writebuflen = 0;
  return true;
}

static bool file_read_peek(FILE *file) __requires_exclusive(*file) {
  if (!file_drain(file))
    return false;

  // Read data.
  ssize_t ret =
      pread(file->fd, file->file.buf, file->file.bufsize, file->offset);
  if (ret < 0)
    return false;

  // Put new read buffer in place.
  file->readbuf = file->file.buf;
  file->readbuflen = ret;
  file->offset += file->readbuflen;
  return true;
}

static bool file_write_peek(FILE *file) __requires_exclusive(*file) {
  if (!file_drain(file))
    return false;

  // Put new write buffer in place.
  file->writebuf = file->file.written = file->file.buf;
  file->writebuflen = file->file.bufsize;
  file->offset += file->writebuflen;
  return true;
}

static bool file_seek(FILE *file, off_t offset, bool seek_end)
    __requires_exclusive(*file) {
  // Drain data that needs to be written first. This may extend the
  // length of the file.
  if (!file_drain(file))
    return false;

  // Adjust the offset to be absolute.
  if (seek_end) {
    struct stat sb;
    if (fstat(file->fd, &sb) != 0)
      return false;
    if (add_overflow(offset, sb.st_size, &offset)) {
      errno = EOVERFLOW;
      return false;
    }
  }

  // Disallow negative offsets.
  if (offset < 0) {
    errno = EINVAL;
    return false;
  }

  // Apply new offset.
  file->offset = offset;
  return true;
}

static bool file_setvbuf(FILE *file, size_t size) __requires_exclusive(*file) {
  if (!file_drain(file))
    return false;

  // Grow/shrink buffer using realloc().
  char *new_buf = realloc(file->file.buf, size);
  if (new_buf == NULL)
    return false;
  file->file.buf = new_buf;
  file->file.bufsize = size;
  return true;
}

static bool file_flush(FILE *file) __requires_exclusive(*file) {
  // Write data that still needs to written to disk. Update the offset
  // of the file descriptor to be in sync with the stream.
  if (!file_drain(file))
    return false;
  return lseek(file->fd, file->offset, SEEK_SET) >= 0;
}

static bool file_close(FILE *file) __requires_exclusive(*file) {
  // Drain any data that still needs to be written.
  bool okay = true;
  if (!file_drain(file))
    okay = false;

  // Free read/write buffer and the underlying file descriptor.
  free(file->file.buf);
  if (close(file->fd) != 0)
    okay = false;
  return okay;
}

static FILE *file_open(int fildes, int oflags, locale_t locale, off_t offset) {
  DECLARE_FILEOPS_ACCMODE(file);

  // Allocate the read/write buffer.
  char *buf = malloc(BUFSIZ);
  if (buf == NULL)
    return NULL;

  // Allocate new stream object and initialize it.
  FILE *file = __falloc(locale);
  if (file == NULL) {
    free(buf);
    return NULL;
  }
  file->fd = fildes;
  file->file.buf = buf;
  file->file.bufsize = BUFSIZ;
  file->file.append = (oflags & O_APPEND) != 0;
  file->offset = offset;
  file->ops = GET_FILEOPS_ACCMODE(file, oflags);
  return file;
}

// Operations for descriptors that cannot be seeked (i.e., pipes, sockets).
//
// Pipes and sockets require us to have separate read/write buffers. We
// should also use read() and write(), as we cannot write at a position.

static bool pipe_read_peek(FILE *file) __requires_exclusive(*file) {
  // Read next chunk of data.
  ssize_t ret = read(file->fd, file->pipe.readbuf, file->pipe.bufsize);
  if (ret < 0)
    return false;
  file->readbuf = file->pipe.readbuf;
  file->readbuflen += ret;
  return true;
}

static bool pipe_write_peek(FILE *file) __requires_exclusive(*file) {
  // Check whether there is data that still needs to be written to disk.
  while (file->pipe.written < file->writebuf) {
    ssize_t ret = write(file->fd, file->pipe.written,
                        file->writebuf - file->pipe.written);
    if (ret < 0) {
      file->flags |= F_ERROR;
      return false;
    }
    file->pipe.written += ret;
  }

  // Reset the write buffer.
  file->writebuf = file->pipe.written = file->pipe.writebuf;
  file->writebuflen = file->pipe.bufsize;
  return true;
}

#define pipe_seek espipe

static bool pipe_setvbuf(FILE *file, size_t size) __requires_exclusive(*file) {
  // Disallow resizing the buffers if it means we'd have to throw away data.
  size_t readbuf_used = file->readbuflen;
  size_t writebuf_used = file->writebuf - file->pipe.written;
  if (size < readbuf_used || size < writebuf_used)
    return false;

  // Allocate new read/write buffers.
  char *new_readbuf = malloc(size);
  if (new_readbuf == NULL)
    return false;
  char *new_writebuf = malloc(size);
  if (new_writebuf == NULL) {
    free(new_readbuf);
    return false;
  }

  // Copy data from the existing read/write buffers and discard those.
  memcpy(new_readbuf, file->readbuf, readbuf_used);
  free(file->pipe.readbuf);
  memcpy(new_writebuf, file->pipe.written, writebuf_used);
  free(file->pipe.writebuf);

  // Install new buffer.
  file->pipe.readbuf = new_readbuf;
  file->pipe.writebuf = file->pipe.written = new_writebuf;
  file->pipe.bufsize = size;

  // Update buffer accessors.
  file->readbuf = file->pipe.readbuf;
  file->readbuflen = readbuf_used;
  file->writebuf = file->pipe.writebuf + writebuf_used;
  file->writebuflen = file->pipe.bufsize - writebuf_used;
  return true;
}

static bool pipe_flush(FILE *file) __requires_exclusive(*file) {
  return pipe_write_peek(file);
}

static bool pipe_close(FILE *file) __requires_exclusive(*file) {
  // Drain any data that still needs to be written.
  bool okay = true;
  if (!pipe_write_peek(file))
    okay = false;

  // Free read/write buffers and the underlying file descriptor.
  free(file->pipe.readbuf);
  free(file->pipe.writebuf);
  if (close(file->fd) != 0)
    okay = false;
  return okay;
}

static FILE *pipe_open(int fildes, int oflags, locale_t locale) {
  DECLARE_FILEOPS_ACCMODE(pipe);

  // Allocate the read/write buffers.
  char *readbuf = malloc(BUFSIZ);
  if (readbuf == NULL)
    return NULL;
  char *writebuf = malloc(BUFSIZ);
  if (writebuf == NULL) {
    free(readbuf);
    return NULL;
  }

  // Allocate new stream object and initialize it.
  FILE *file = __falloc(locale);
  if (file == NULL) {
    free(readbuf);
    free(writebuf);
    return NULL;
  }
  file->fd = fildes;
  file->offset = -1;
  file->ops = GET_FILEOPS_ACCMODE(pipe, oflags);
  file->pipe.readbuf = readbuf;
  file->pipe.writebuf = writebuf;
  file->pipe.bufsize = BUFSIZ;
  return file;
}

FILE *fdopen_l(int fildes, const char *mode, locale_t locale) {
  // Parse mode string.
  int oflags = get_oflags_from_string(mode);
  if (oflags == 0) {
    errno = EINVAL;
    return NULL;
  }

  // Determine whether this file descriptor is seekable. Based on this
  // we either associate it with the file operations or the pipe
  // operations.
  off_t offset = lseek(fildes, 0, SEEK_CUR);
  if (offset == -1) {
    if (errno != ESPIPE)
      return NULL;
    // Descriptor is a pipe, socket, etc.
    return pipe_open(fildes, oflags, locale);
  } else {
    // Descriptor is a regular file.
    return file_open(fildes, oflags, locale, offset);
  }
}
