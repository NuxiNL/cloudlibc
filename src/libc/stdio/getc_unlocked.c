// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/stdio.h>

#include <stdio.h>

#ifndef getc_unlocked
#error "getc_unlocked is supposed to be a macro as well"
#endif

// clang-format off
int (getc_unlocked)(FILE *stream) {
  return getc_unlocked(stream);
}
