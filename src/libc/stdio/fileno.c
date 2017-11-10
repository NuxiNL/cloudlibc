// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

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
