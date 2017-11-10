// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <signal.h>

#ifndef sigemptyset
#error "sigemptyset is supposed to be a macro as well"
#endif

// clang-format off
int (sigemptyset)(sigset_t *set) {
  return sigemptyset(set);
}
