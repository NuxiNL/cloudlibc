// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdio.h>

#ifndef fseek
#error "fseek is supposed to be a macro as well"
#endif

// clang-format off
int (fseek)(FILE *stream, long offset, int whence) {
  return fseek(stream, offset, whence);
}
