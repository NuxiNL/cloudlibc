// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdatomic.h>
#include <stdbool.h>

#ifndef atomic_flag_test_and_set_explicit
#error "atomic_flag_test_and_set_explicit is supposed to be a macro as well"
#endif

// clang-format off
bool (atomic_flag_test_and_set_explicit)(volatile atomic_flag *object,
                                         memory_order order) {
  return atomic_flag_test_and_set_explicit(object, order);
}
