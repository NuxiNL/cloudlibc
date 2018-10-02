// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdlib.h>
#include <cloudlibc_interceptors.h>

#ifndef atoi
#error "atoi is supposed to be a macro as well"
#endif

// clang-format off
int __cloudlibc_atoi(const char *str) {
  return atoi(str);
}

__weak_reference(__cloudlibc_atoi, atoi);
