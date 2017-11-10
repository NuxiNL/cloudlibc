// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>
#include <common/stdio.h>

#include <fcntl.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>

#define stderr_read_peek ebadf

static bool stderr_write_peek(FILE *file) {
  // Provide a '/dev/null' buffer.
  static char nullbuf[128];
  file->writebuf = nullbuf;
  file->writebuflen = sizeof(nullbuf);
  return true;
}

#define stderr_seek espipe

static bool stderr_setvbuf(FILE *file, size_t size) {
  // Ignore request.
  return true;
}

static bool stderr_flush(FILE *file) {
  // Nothing to flush.
  return true;
}

static bool stderr_close(FILE *file) {
  // Nothing to free.
  return true;
}

DECLARE_FILEOPS_SIMPLE(stderr);

struct _FILE __stderr = {
    .fd = -1,
    .ops = GET_FILEOPS_SIMPLE(stderr),
    .ctype = &__ctype_us_ascii,

    .lock = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP,
    .buftype = _IOFBF,
    .offset = -1,
};
