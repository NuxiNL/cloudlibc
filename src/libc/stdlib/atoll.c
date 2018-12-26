// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdlib.h>
#include <cloudlibc_interceptors.h>

#ifndef atoll
#error "atoll is supposed to be a macro as well"
#endif

// clang-format off
long long __cloudlibc_atoll(const char *str) {
  return atoll(str);
}

__weak_reference(__cloudlibc_atoll, atoll);
