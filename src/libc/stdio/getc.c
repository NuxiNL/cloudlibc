// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/stdio.h>

#include <stdio.h>

int getc(FILE *stream) {
  flockfile(stream);
  int result = getc_unlocked(stream);
  funlockfile(stream);
  return result;
}

__strong_reference(getc, fgetc);
