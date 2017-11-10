// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdatomic.h>
#include <stdbool.h>

#ifndef atomic_flag_test_and_set
#error "atomic_flag_test_and_set is supposed to be a macro as well"
#endif

// clang-format off
bool (atomic_flag_test_and_set)(volatile atomic_flag *object) {
  return atomic_flag_test_and_set(object);
}
