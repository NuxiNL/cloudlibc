// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/stdio.h>

#include <stdio.h>

int putc(int c, FILE *stream) {
  flockfile_orientation(stream, -1);
  int result = putc_unlocked(c, stream);
  funlockfile(stream);
  return result;
}

__strong_reference(putc, fputc);
