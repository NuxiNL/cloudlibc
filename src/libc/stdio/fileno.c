// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/stdio.h>

#include <errno.h>
#include <stdio.h>

int fileno(FILE *stream) {
  flockfile(stream);
  int fd = stream->fd;
  funlockfile(stream);
  if (fd < 0) {
    errno = EBADF;
    return -1;
  }
  return fd;
}
