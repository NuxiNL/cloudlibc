// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdlib.h>
#include <cloudlibc_interceptors.h>

#ifndef atol
#error "atol is supposed to be a macro as well"
#endif

// clang-format off
long __cloudlibc_atol(const char *str) {
  return atol(str);
}

__weak_reference(__cloudlibc_atol, atol);
