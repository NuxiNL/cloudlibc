// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/overflow.h>
#include <common/stdio.h>

#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void mem_drain(FILE *file) __requires_exclusive(*file) {
  // TODO(ed): Perform NULL termination.

  // Sync up the file offset again.
  file->offset = ftello_physical(file);

  // Adjust the file length if we've written data.
  if (file->writebuf != file->fmemopen.owritebuf &&
      (off_t)file->fmemopen.used < file->offset) {
    // Write of data may have introduced sparseness into the file. Zero
    // out the sparse region.
    const char *end_sparse = file->fmemopen.owritebuf;
    char *start_sparse = file->fmemopen.buf + file->fmemopen.used;
    if (start_sparse < end_sparse)
      memset(start_sparse, '\0', end_sparse - start_sparse);

    file->fmemopen.used = file->offset;
  }

  // Discard both the read and write buffers.
  file->readbuf = NULL;
  file->readbuflen = 0;
  file->fmemopen.owritebuf = file->writebuf = NULL;
  file->writebuflen = 0;
}

static bool mem_read_peek(FILE *file) __requires_exclusive(*file) {
  mem_drain(file);

  // Expose the read buffer if the offset is not beyond end of file.
  if (file->offset < (off_t)file->fmemopen.used) {
    file->readbuf = file->fmemopen.buf + file->offset;
    file->readbuflen = file->fmemopen.used - file->offset;
    file->offset = file->fmemopen.used;
  }
  return true;
}

static bool mem_write_peek(FILE *file) __requires_exclusive(*file) {
  mem_drain(file);

  // Reset position to the end if we're using O_APPEND.
  if (file->fmemopen.append)
    file->offset = file->fmemopen.used;

  // Disallow writes beyond end of file.
  if (file->offset >= (off_t)file->fmemopen.size) {
    errno = ENOSPC;
    return false;
  }

  // Expose the write buffer.
  file->fmemopen.owritebuf = file->writebuf = file->fmemopen.buf + file->offset;
  file->writebuflen = file->fmemopen.size - file->offset;
  file->offset = file->fmemopen.size;
  return true;
}

static bool mem_seek(FILE *file, off_t offset, bool seek_end)
    __requires_exclusive(*file) {
  // Force a drain to adjust the offset and length of the file.
  mem_drain(file);

  // Adjust the offset to be absolute.
  if (seek_end) {
    if (add_overflow(offset, file->fmemopen.used, &offset)) {
      errno = EOVERFLOW;
      return false;
    }
  }

  // Disallow negative offsets.
  if (offset < 0 || offset > (off_t)file->fmemopen.size) {
    errno = EINVAL;
    return false;
  }

  // Apply new offset.
  file->offset = offset;
  return true;
}

static bool mem_setvbuf(FILE *file, size_t size) __requires_exclusive(*file) {
  // Buffer size of memory files cannot be adjusted, as data is written
  // into the underlying buffer directly.
  return true;
}

static bool mem_flush(FILE *file) __requires_exclusive(*file) {
  // Perform drain to force null termination and zeroing of sparse space.
  mem_drain(file);
  return true;
}

static bool mem_close(FILE *file) __requires_exclusive(*file) {
  if (file->fmemopen.external) {
    // External buffer. Perform drain to force null termination and
    // zeroing of sparse space.
    mem_drain(file);
  } else {
    // Internal buffer. Simply free the contents.
    free(file->fmemopen.buf);
  }
  return true;
}

FILE *fmemopen_l(void *restrict buf, size_t size, const char *restrict mode,
                 locale_t locale) {
  DECLARE_FILEOPS_ACCMODE(mem);

  // Process arguments. Zero-sized buffers are not permitted.
  int oflags = get_oflags_from_string(mode);
  if (oflags == 0 || size == 0) {
    errno = EINVAL;
    return NULL;
  }

  if (size == 0) {
    errno = EINVAL;
    return NULL;
  }

  // Allocate buffer and FILE object.
  FILE *file;
  if (buf == NULL && size > 0) {
    buf = malloc(size);
    if (buf == NULL)
      return NULL;
    file = __falloc(locale);
    if (file == NULL) {
      free(buf);
      return NULL;
    }
  } else {
    file = __falloc(locale);
    if (file == NULL)
      return NULL;
    file->fmemopen.external = true;

    if ((oflags & O_APPEND) != 0) {
      // Set offset to the end of the data.
      file->offset = file->fmemopen.used = strnlen(buf, size);
    } else if ((oflags & O_WRONLY) == 0) {
      // Use provided size directly.
      file->fmemopen.used = size;
    }
  }

  file->fmemopen.buf = buf;
  file->fmemopen.size = size;
  file->fmemopen.append = (oflags & O_APPEND) != 0;
  file->writebuf = file->fmemopen.buf + file->offset;
  file->ops = GET_FILEOPS_ACCMODE(mem, oflags);
  return file;
}
