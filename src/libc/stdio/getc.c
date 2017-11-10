// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/stdio.h>

#include <stdio.h>

int getc(FILE *stream) {
  flockfile_orientation(stream, -1);
  int result = getc_unlocked(stream);
  funlockfile(stream);
  return result;
}

__strong_reference(getc, fgetc);
