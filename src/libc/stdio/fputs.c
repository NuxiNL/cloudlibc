// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/stdio.h>

#include <stdio.h>
#include <string.h>

int fputs(const char *restrict s, FILE *restrict stream) {
  // Compute input length.
  size_t len = strlen(s);
  if (len == 0)
    return 0;

  // Write it.
  flockfile(stream);
  size_t written = fwrite_put(stream, s, len);
  funlockfile(stream);
  return written == len ? 0 : EOF;
}
