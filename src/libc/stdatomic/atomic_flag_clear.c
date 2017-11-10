// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdatomic.h>

#ifndef atomic_flag_clear
#error "atomic_flag_clear is supposed to be a macro as well"
#endif

// clang-format off
void (atomic_flag_clear)(volatile atomic_flag *object) {
  return atomic_flag_clear(object);
}
