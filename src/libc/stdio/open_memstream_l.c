// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/overflow.h>
#include <common/stdio.h>

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void ms_drain(FILE *file) __requires_exclusive(*file) {
  // Sync up the file offset again.
  file->offset = ftello_physical(file);

  // Adjust the file length if we've written data.
  if (file->writebuf != file->memstream.owritebuf &&
      (off_t)file->memstream.used < file->offset) {
    // Write of data may have introduced sparseness into the file. Zero
    // out the sparse region.
    const char *end_sparse = file->memstream.owritebuf;
    char *start_sparse = file->memstream.buf + file->memstream.used;
    if (start_sparse < end_sparse)
      memset(start_sparse, '\0', end_sparse - start_sparse);

    file->memstream.used = file->offset;
  }

  // Discard the write buffer.
  file->memstream.owritebuf = file->writebuf = NULL;
  file->writebuflen = 0;
}

#define ms_read_peek ebadf

static bool ms_write_peek(FILE *file) __requires_exclusive(*file) {
  ms_drain(file);

  // Grow the file buffer if the cursor is at end of file or beyond.
  // We ensure that we always allocate one byte more than what is stored
  // in file->memstream.size, so that it's always safe to add a trailing
  // null byte in ms_flush().
  if (file->offset >= (off_t)file->memstream.size) {
    size_t new_size = file->memstream.size + 1;
    while (file->offset >= (off_t)new_size - 1)
      new_size *= 2;
    char *new_buf = realloc(file->memstream.buf, new_size);
    if (new_buf == NULL)
      return false;
    file->memstream.buf = new_buf;
    file->memstream.size = new_size - 1;
  }

  // Expose the write buffer.
  file->memstream.owritebuf = file->writebuf =
      file->memstream.buf + file->offset;
  file->writebuflen = file->memstream.size - file->offset;
  file->offset = file->memstream.size;
  return true;
}

static bool ms_seek(FILE *file, off_t offset, bool seek_end)
    __requires_exclusive(*file) {
  // Force a drain to adjust the offset and length of the file.
  ms_drain(file);

  // Adjust the offset to be absolute.
  if (seek_end) {
    if (add_overflow(offset, file->memstream.used, &offset)) {
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

static bool ms_setvbuf(FILE *file, size_t size) __requires_exclusive(*file) {
  // Buffer size of memory files cannot be adjusted, as data is written
  // into the underlying buffer directly.
  return true;
}

static bool ms_flush(FILE *file) __requires_exclusive(*file) {
  // Perform drain to force zeroing of sparse space.
  ms_drain(file);

  // Perform null termination and expose the buffer.
  file->memstream.buf[file->memstream.used] = '\0';
  *file->memstream.bufp = file->memstream.buf;
  *file->memstream.sizep = (off_t)file->memstream.used < file->offset
                               ? file->memstream.used
                               : file->offset;
  return true;
}

#define ms_close ms_flush

#define DEFAULT_BUFFER_SIZE (2 * sizeof(void *))

FILE *open_memstream_l(char **bufp, size_t *sizep, locale_t locale) {
  DECLARE_FILEOPS_SIMPLE(ms);

  // Already allocate a buffer upon creation. This ensures that fflush()
  // and fclose() may never fail.
  char *buf = malloc(DEFAULT_BUFFER_SIZE);
  if (buf == NULL)
    return NULL;
  FILE *file = __falloc(locale);
  if (file == NULL) {
    free(buf);
    return NULL;
  }
  file->ops = GET_FILEOPS_SIMPLE(ms);
  file->memstream.buf = buf;
  file->memstream.size = DEFAULT_BUFFER_SIZE - 1;
  file->memstream.bufp = bufp;
  file->memstream.sizep = sizep;
  return file;
}
