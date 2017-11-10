// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdio.h>

#ifndef setbuf
#error "setbuf is supposed to be a macro as well"
#endif

// clang-format off
void (setbuf)(FILE *restrict stream, char *restrict buf) {
  setbuf(stream, buf);
}
