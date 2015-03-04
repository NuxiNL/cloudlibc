// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/stdio.h>

#include <stdio.h>

#ifndef funlockfile
#error "funlockfile is supposed to be a macro as well"
#endif

// clang-format off
void (funlockfile)(FILE *file) {
  funlockfile(file);
}
