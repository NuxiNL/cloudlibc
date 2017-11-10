// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

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
