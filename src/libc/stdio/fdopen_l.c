// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/limits.h>
#include <common/stdio.h>

#include <sys/stat.h>

#include <locale.h>
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

static bool file_write_flush(FILE *);

static bool file_read_peek(FILE *file) {
  assert((file->oflags & O_RDONLY) != 0 &&
         "Attempted to read from non-readable file");

  // First ensure that there's no more data in the write buffer before
  // attempting to read.
  if (!file_write_flush(file))
    return false;

  // Recompute current file offset.
  assert(file->readbuflen == 0 && "Read buffer available after write flush");
  assert(file->writebuf == file->file.buf && "Not all data has been flushed");
  off_t offset = file->offset - file->writebuflen;
  assert(offset >= 0 && "File offset went negative");

  // Read data.
  ssize_t ret = pread(file->fd, file->file.buf, file->file.bufsize, offset);
  if (ret < 0)
    return false;

  // Put new read buffer in place.
  file->readbuf = file->file.buf;
  file->readbuflen = ret;
  file->writebuflen = 0;
  file->offset = offset + file->readbuflen;
  return true;
}

static bool file_write_flush(FILE *file) {
  const char *buf = file->file.buf;
  for (;;) {
    size_t length = file->writebuf - buf;
    if (length == 0)
      break;

    assert((file->oflags & O_WRONLY) != 0 &&
           "Attempted to write to non-writable file");
    assert(file->readbuflen == 0 &&
           "Read and write buffer in use simultaneously");
    off_t offset = file->offset - file->writebuflen - length;
    ssize_t ret = pwrite(file->fd, buf, length, offset);
    if (ret < 0) {
      // Failed to write data. Move unwritten data to the start ouf the
      // write buffer so it can be retried later on.
      memmove(file->file.buf, buf, length);
      file->writebuf = file->file.buf + length;
      file->writebuflen = file->file.bufsize - length;
      file->offset = offset + file->file.bufsize;
      return false;
    }
    buf += ret;
  }

  // No data left to write. Reset the write buffer.
  off_t offset = file->offset - file->readbuflen - file->writebuflen;
  file->readbuflen = 0;
  file->writebuf = file->file.buf;
  file->writebuflen = file->file.bufsize;
  file->offset = offset + file->writebuflen;
  return true;
}

static bool file_seek(FILE *file, off_t offset, int whence) {
  // Determine new file offset.
  off_t base;
  switch (whence) {
    case SEEK_CUR: {
      // Add current position within the file.
      base = file->offset - file->readbuflen - file->writebuflen;
      break;
    }
    case SEEK_END: {
      // Add length of the file.
      struct stat sb;
      if (fstat(file->fd, &sb) != 0)
        return false;
      base = sb.st_size;
      break;
    }
    case SEEK_SET: {
      // Absolute offset.
      base = 0;
      break;
    }
    default: {
      errno = EINVAL;
      return false;
    }
  }

  // The condition below performs bounds checking against the offset.
  // The expression essentially computes:
  //
  //     offset + base < 0 ||
  //     offset + base + file->file.bufsize > NUMERIC_MAX(off_t)
  assert(base >= 0 && "Negative base offset");
  if (offset < -base ||
      offset > NUMERIC_MAX(off_t) - base - (off_t)file->file.bufsize) {
    errno = EINVAL;
    return false;
  }

  // Flush read/write buffers before seeking.
  if (!file_write_flush(file))
    return false;

  // Apply the new offset.
  assert(file->writebuflen == file->file.bufsize &&
         "Write buffer is not in its initial state");
  file->offset = base + offset + file->writebuflen;
  return true;
}

static bool file_setvbuf(FILE *file, size_t size) {
  // TODO(edje): Implement.
  errno = ENOSYS;
  return false;
}

static FILE *file_open(int fildes, const char *mode, locale_t locale,
                       off_t offset) {
  static const struct fileops ops = {
      .read_peek = file_read_peek,
      .write_flush = file_write_flush,
      .seek = file_seek,
      .setvbuf = file_setvbuf,
  };

  // Allocate the read/write buffer.
  char *buf = malloc(BUFSIZ);
  if (buf == NULL)
    return NULL;

  // Allocate new stream object and initialize it.
  FILE *file = __falloc(mode, locale);
  if (file == NULL) {
    free(buf);
    return NULL;
  }
  file->fd = fildes;
  // TODO(edje): Have separate ops for the append case.
  file->ops = &ops;

  // Attach allocated buffer.
  file->file.buf = buf;
  file->file.bufsize = BUFSIZ;

  // Initial state: empty read and write buffers.
  file->readbuflen = 0;
  file->writebuf = file->file.buf;
  file->writebuflen = file->file.bufsize;
  file->offset = offset + BUFSIZ;
  return file;
}

// Operations for descriptors that cannot be seeked (i.e., pipes, sockets).
//
// Pipes and sockets require us to have separate read/write buffers. We
// should also use read() and write(), as we cannot write at a position.

static bool pipe_read_peek(FILE *file) {
  assert((file->oflags & O_RDONLY) != 0 &&
         "Attempted to read from non-readable file");

  // Move remaining data in the read buffer to the front.
  memmove(file->pipe.readbuf, file->readbuf, file->readbuflen);
  file->readbuf = file->pipe.readbuf;

  // Read next chunk of data.
  ssize_t ret = read(file->fd, file->readbuf + file->readbuflen,
                     file->pipe.bufsize - file->readbuflen);
  if (ret < 0)
    return false;

  // Successfully read next buffer of data.
  file->readbuflen += ret;
  return true;
}

static bool pipe_write_flush(FILE *file) {
  const char *buf = file->pipe.writebuf;
  for (;;) {
    size_t length = file->writebuf - buf;
    if (length == 0)
      break;

    assert((file->oflags & O_WRONLY) != 0 &&
           "Attempted to write to non-writable file");
    ssize_t ret = write(file->fd, buf, length);
    if (ret < 0) {
      // Failed to write data. Move unwritten data to the start ouf the
      // write buffer so it can be retried later on.
      memmove(file->pipe.writebuf, buf, length);
      file->writebuf = file->pipe.writebuf + length;
      file->writebuflen = file->pipe.bufsize - length;
      return false;
    }
    buf += ret;
  }

  // No data left to write. Reset the write buffer.
  file->writebuf = file->pipe.writebuf;
  file->writebuflen = file->pipe.bufsize;
  return true;
}

static bool pipe_seek(FILE *file, off_t offset, int whence) {
  // We cannot seek in pipes.
  errno = ESPIPE;
  return false;
}

static bool pipe_setvbuf(FILE *file, size_t bufsize) {
  // TODO(edje): Implement.
  return true;
}

static FILE *pipe_open(int fildes, const char *mode, locale_t locale) {
  static const struct fileops ops = {
      .read_peek = pipe_read_peek,
      .write_flush = pipe_write_flush,
      .seek = pipe_seek,
      .setvbuf = pipe_setvbuf,
  };

  // Allocate the read/write buffer.
  char *buf = malloc(2 * BUFSIZ);
  if (buf == NULL)
    return NULL;

  // Allocate new stream object and initialize it.
  FILE *file = __falloc(mode, locale);
  if (file == NULL) {
    free(buf);
    return NULL;
  }
  file->fd = fildes;
  file->ops = &ops;
  file->offset = -1;

  // Attach allocated buffer.
  file->pipe.readbuf = buf;
  file->pipe.writebuf = buf + BUFSIZ;
  file->pipe.bufsize = BUFSIZ;

  // Initial state: empty read and write buffers.
  file->readbuflen = 0;
  file->writebuf = file->pipe.writebuf;
  file->writebuflen = file->pipe.bufsize;
  return file;
}

FILE *fdopen_l(int fildes, const char *mode, locale_t locale) {
  // Determine whether this file descriptor is seekable. Based on this
  // we either associate it with the file operations or the pipe
  // operations.
  off_t offset = lseek(fildes, 0, SEEK_CUR);
  if (offset == -1) {
    if (errno != ESPIPE)
      return NULL;
    // Descriptor is a pipe, socket, etc.
    return pipe_open(fildes, mode, locale);
  } else {
    // Descriptor is a regular file.
    return file_open(fildes, mode, locale, offset);
  }
}
