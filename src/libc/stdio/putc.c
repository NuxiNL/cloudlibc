// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/stdio.h>

#include <stdio.h>

int putc(int c, FILE *stream) {
  flockfile_orientation(stream, -1);
  int result = putc_unlocked(c, stream);
  funlockfile(stream);
  return result;
}

__strong_reference(putc, fputc);
