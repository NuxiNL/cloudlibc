// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdatomic.h>

#ifndef atomic_flag_clear_explicit
#error "atomic_flag_clear_explicit is supposed to be a macro as well"
#endif

// clang-format off
void (atomic_flag_clear_explicit)(volatile atomic_flag *object,
                                  memory_order order) {
  return atomic_flag_clear_explicit(object, order);
}
