// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdio.h>

int putc(int c, FILE *stream) {
  flockfile(stream);
  int result = putc_unlocked(c, stream);
  funlockfile(stream);
  return result;
}

__strong_reference(putc, fputc);
