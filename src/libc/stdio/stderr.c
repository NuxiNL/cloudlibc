// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>
#include <common/stdio.h>

#include <fcntl.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>

static bool stderr_write_peek(FILE *file) {
  // Provide a '/dev/null' buffer.
  static char nullbuf[128];
  file->writebuf = nullbuf;
  file->writebuflen = sizeof(nullbuf);
  return true;
}

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

static struct fileops stderrops = {
    .write_peek = stderr_write_peek,
    .setvbuf = stderr_setvbuf,
    .flush = stderr_flush,
    .close = stderr_close,
};

struct _FILE __stderr = {
    .fd = -1,
    .oflags = O_WRONLY,
    .ops = &stderrops,
    .ctype = &__ctype_us_ascii,

    .lock = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP,
    .buftype = _IOFBF,
    .offset = -1,
};
