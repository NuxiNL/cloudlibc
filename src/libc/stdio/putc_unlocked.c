// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/stdio.h>

#include <stdio.h>

#ifndef putc_unlocked
#error "putc_unlocked is supposed to be a macro as well"
#endif

// clang-format off
int (putc_unlocked)(int c, FILE *stream) {
  return putc_unlocked(c, stream);
}
