// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/stdio.h>

#include <stdio.h>

#ifndef getc_unlocked
#error "getc_unlocked is supposed to be a macro as well"
#endif

// clang-format off
int (getc_unlocked)(FILE *stream) {
  return getc_unlocked(stream);
}
