// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/locale.h>
#include <common/stdio.h>

#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>

static bool stderr_write_flush(FILE *file) {
  // Provide a '/dev/null' buffer.
  static char nullbuf[128];
  file->writebuf = nullbuf;
  file->writebuflen = sizeof(nullbuf);
  return true;
}

static bool stderr_seek(FILE *file, off_t offset, int whence) {
  // Behave like a pipe.
  errno = ESPIPE;
  return false;
}

static bool stderr_setvbuf(FILE *file, size_t size) {
  // Ignore request.
  return true;
}

static struct fileops stderrops = {
    .write_flush = stderr_write_flush,
    .seek = stderr_seek,
    .setvbuf = stderr_setvbuf,
};

static struct _FILE stderrobj = {
    .fd = -1,
    .oflags = O_WRONLY,
    .ops = &stderrops,
    .ctype = &__ctype_us_ascii,

    .lock = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP,
    .buftype = _IOFBF,
    .offset = -1,
};

// Initialize stderr with a FILE object that discards output.
FILE *stderr = &stderrobj;
