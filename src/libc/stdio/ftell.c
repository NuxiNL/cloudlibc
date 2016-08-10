// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <limits.h>
#include <stdio.h>

long ftell(FILE *stream) {
  off_t off = ftello(stream);
  if (off > LONG_MAX) {
    // Result would get truncated.
    errno = EOVERFLOW;
    return -1;
  }
  return off;
}
