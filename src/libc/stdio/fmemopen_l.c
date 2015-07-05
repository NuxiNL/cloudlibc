// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/stdio.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static bool mem_read_peek(FILE *file) {
  if (file->offset < (off_t)file->fmemopen.used) {
    file->readbuf = file->fmemopen.buf + file->offset;
    file->readbuflen = file->fmemopen.used - file->offset;
    file->offset = file->fmemopen.used;
  }
  return true;
}

// Single-byte space, used as a write buffer when attempting to write
// beyond end-of-file.
static char dead_space[1];

static bool mem_write_flush(FILE *file) {
  if (file->writebuf == &dead_space[0]) {
    // Nothing happened.
    return true;
  } else if (file->writebuf == &dead_space[1]) {
    // Attempted to write beyond end of buffer.
    file->writebuf = &dead_space[0];
    file->writebuflen = 1;
    file->offset = file->fmemopen.size + 1;
    errno = ENOSPC;
    return false;
  } else {
    // Determine current offset within the file.
    size_t offset = file->writebuf - file->fmemopen.buf;

    // TODO(ed): Is this logic correct?
    if (offset > file->fmemopen.used)
      file->fmemopen.used = file->offset;

    file->writebuflen = file->fmemopen.size - offset;
    if (file->writebuflen == 0) {
      // We're at the very end of our memory file. Provide a trap byte
      // that will be used to determine whether we're writing past the
      // end of the file.
      file->writebuf = &dead_space[0];
      file->writebuflen = 1;
      file->offset = file->fmemopen.size + 1;
    } else {
      // Return the region from current position to the end of the file.
      file->writebuf = file->fmemopen.buf + offset;
      file->offset = file->fmemopen.size;
    }
    return true;
  }
}

static bool mem_setvbuf(FILE *stream, size_t size) {
  // Buffer size of memory files cannot be adjusted, as data is written
  // into the underlying buffer directly.
  return true;
}

static const struct fileops mem_fileops = {
    .read_peek = mem_read_peek,
    .write_flush = mem_write_flush,
    .setvbuf = mem_setvbuf,
};

FILE *fmemopen_l(void *restrict buf, size_t size, const char *restrict mode,
                 locale_t locale) {
  // Allocate buffer and FILE object.
  FILE *file;
  if (buf == NULL && size > 0) {
    buf = malloc(size);
    if (buf == NULL)
      return NULL;
    file = __falloc(mode, locale);
    if (file == NULL) {
      free(buf);
      return NULL;
    }
  } else {
    file = __falloc(mode, locale);
    if (file == NULL)
      return NULL;

    if (file->oflags & O_APPEND) {
      // Set offset to the end of the data.
      file->offset = file->fmemopen.used = strnlen(buf, size);
    } else if ((file->oflags & O_WRONLY) == 0) {
      // Use provided size directly.
      file->fmemopen.used = size;
    }
  }

  file->fmemopen.buf = buf;
  file->fmemopen.size = size;
  file->writebuf = file->fmemopen.buf + file->offset;
  file->ops = &mem_fileops;
  return file;
}
