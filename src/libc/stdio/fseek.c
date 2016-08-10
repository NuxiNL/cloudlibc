// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdio.h>

#ifndef fseek
#error "fseek is supposed to be a macro as well"
#endif

// clang-format off
int (fseek)(FILE *stream, long offset, int whence) {
  return fseek(stream, offset, whence);
}
