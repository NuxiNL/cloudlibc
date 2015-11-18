// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/stdio.h>

#include <stdio.h>
#include <string.h>

int fputs(const char *restrict s, FILE *restrict stream) {
  // Compute input length.
  flockfile_orientation(stream, -1);
  size_t len = strlen(s);
  if (len == 0) {
    funlockfile(stream);
    return 0;
  }

  // Write it.
  size_t written = fwrite_put(stream, s, len);
  funlockfile(stream);
  return written == len ? 0 : EOF;
}
