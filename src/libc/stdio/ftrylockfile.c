// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/stdio.h>

#include <stdio.h>

#ifndef ftrylockfile
#error "ftrylockfile is supposed to be a macro as well"
#endif

// clang-format off
int (ftrylockfile)(FILE *file) {
  return ftrylockfile(file);
}
