// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdatomic.h>

#ifndef atomic_flag_clear_explicit
#error "atomic_flag_clear_explicit is supposed to be a macro as well"
#endif

// clang-format off
void (atomic_flag_clear_explicit)(volatile atomic_flag *object,
                                  memory_order order) {
  return atomic_flag_clear_explicit(object, order);
}
