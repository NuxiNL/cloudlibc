// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdatomic.h>
#include <stdbool.h>

#ifndef atomic_flag_test_and_set
#error "atomic_flag_test_and_set is supposed to be a macro as well"
#endif

// clang-format off
bool (atomic_flag_test_and_set)(volatile atomic_flag *object) {
  return atomic_flag_test_and_set(object);
}
