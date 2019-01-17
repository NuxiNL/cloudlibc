// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <assert.h>
#include <program.h>
#include <stdatomic.h>
#include <stdlib.h>

#define ASSERT_FALSE(a) assert(!(a))
#define ASSERT_TRUE(a) assert(a)

void program_main(const argdata_t *ad) {
  atomic_flag flag = ATOMIC_FLAG_INIT;

  // Test non-explicit functions.
  ASSERT_FALSE(atomic_flag_test_and_set(&flag));
  ASSERT_TRUE(atomic_flag_test_and_set(&flag));
  ASSERT_TRUE(atomic_flag_test_and_set(&flag));
  atomic_flag_clear(&flag);

  // Test explicit functions.
  ASSERT_FALSE(atomic_flag_test_and_set_explicit(&flag, memory_order_relaxed));
  ASSERT_TRUE(atomic_flag_test_and_set_explicit(&flag, memory_order_relaxed));
  ASSERT_TRUE(atomic_flag_test_and_set_explicit(&flag, memory_order_relaxed));
  atomic_flag_clear_explicit(&flag, memory_order_relaxed);

  ASSERT_FALSE(atomic_flag_test_and_set_explicit(&flag, memory_order_relaxed));

  exit(0);
}
