// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/overflow.h>
#include <common/stdio.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static void tmp_drain(FILE *file) __requires_exclusive(*file) {
  // Sync up the file offset again.
  file->offset = ftello_physical(file);

  // Adjust the file length if we've written data.
  if (file->writebuf != file->tmpfile.owritebuf &&
      (off_t)file->tmpfile.used < file->offset) {
    // Write of data may have introduced sparseness into the file. Zero
    // out the sparse region.
    const char *end_sparse = file->tmpfile.owritebuf;
    char *start_sparse = file->tmpfile.buf + file->tmpfile.used;
    if (start_sparse < end_sparse)
      memset(start_sparse, '\0', end_sparse - start_sparse);

    file->tmpfile.used = file->offset;
  }

  // Discard both the read and write buffers.
  file->readbuf = NULL;
  file->readbuflen = 0;
  file->tmpfile.owritebuf = file->writebuf = NULL;
  file->writebuflen = 0;
}

static bool tmp_read_peek(FILE *file) __requires_exclusive(*file) {
  tmp_drain(file);

  // Expose the read buffer if the offset is not beyond end of file.
  if (file->offset < (off_t)file->tmpfile.used) {
    file->readbuf = file->tmpfile.buf + file->offset;
    file->readbuflen = file->tmpfile.used - file->offset;
    file->offset = file->tmpfile.used;
  }
  return true;
}

static bool tmp_write_peek(FILE *file) __requires_exclusive(*file) {
  tmp_drain(file);

  // Grow the file buffer if the cursor is at end of file or beyond.
  if (file->offset >= (off_t)file->tmpfile.size) {
    size_t new_size = file->tmpfile.size;
    if (new_size < 16)
      new_size = 16;
    while (file->offset >= (off_t)new_size)
      new_size *= 2;
    char *new_buf = realloc(file->tmpfile.buf, new_size);
    if (new_buf == NULL)
      return false;
    file->tmpfile.buf = new_buf;
    file->tmpfile.size = new_size;
  }

  // Expose the write buffer.
  file->tmpfile.owritebuf = file->writebuf = file->tmpfile.buf + file->offset;
  file->writebuflen = file->tmpfile.size - file->offset;
  file->offset = file->tmpfile.size;
  return true;
}

static bool tmp_seek(FILE *file, off_t offset, bool seek_end)
    __requires_exclusive(*file) {
  // Force a drain to adjust the offset and length of the file.
  tmp_drain(file);

  // Adjust the offset to be absolute.
  if (seek_end) {
    if (add_overflow(offset, file->tmpfile.used, &offset)) {
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

static bool tmp_setvbuf(FILE *file, size_t size) __requires_exclusive(*file) {
  // Buffer size of memory files cannot be adjusted, as data is written
  // into the underlying buffer directly.
  return true;
}

static bool tmp_flush(FILE *file) __requires_exclusive(*file) {
  // There is no need to flush anything, as the contents of this file
  // are only observable internally.
  return true;
}

static bool tmp_close(FILE *file) __requires_exclusive(*file) {
  free(file->tmpfile.buf);
  return true;
}

FILE *tmpfile_l(locale_t locale) {
  DECLARE_FILEOPS_SIMPLE(tmp);

  FILE *file = __falloc(locale);
  if (file == NULL)
    return NULL;
  file->ops = GET_FILEOPS_SIMPLE(tmp);
  return file;
}
