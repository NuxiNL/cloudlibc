// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/stdio.h>

#include <stdio.h>

#ifndef funlockfile
#error "funlockfile is supposed to be a macro as well"
#endif

// clang-format off
void (funlockfile)(FILE *file) {
  funlockfile(file);
}
