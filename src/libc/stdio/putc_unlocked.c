// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/stdio.h>

#include <stdio.h>

#ifndef putc_unlocked
#error "putc_unlocked is supposed to be a macro as well"
#endif

// clang-format off
int (putc_unlocked)(int c, FILE *stream) {
  return putc_unlocked(c, stream);
}
