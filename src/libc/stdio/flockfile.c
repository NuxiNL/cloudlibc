// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/stdio.h>

#include <stdio.h>

#ifndef flockfile
#error "flockfile is supposed to be a macro as well"
#endif

// clang-format off
void (flockfile)(FILE *file) {
  flockfile(file);
}
