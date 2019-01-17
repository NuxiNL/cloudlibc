// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <assert.h>
#include <program.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdlib.h>
#include <uchar.h>
#include <wchar.h>

#define ASSERT_TRUE(a) assert(a)

void program_main(const argdata_t *ad) {
#define TEST_CONSTANT(constant, type)                                         \
  do {                                                                        \
    type t;                                                                   \
    ASSERT_TRUE((constant == 0 && !atomic_is_lock_free(&t)) ||                \
                constant == 1 || (constant == 2 && atomic_is_lock_free(&t))); \
  } while (0)
  TEST_CONSTANT(ATOMIC_BOOL_LOCK_FREE, bool);
  TEST_CONSTANT(ATOMIC_CHAR_LOCK_FREE, char);
  TEST_CONSTANT(ATOMIC_CHAR16_T_LOCK_FREE, char16_t);
  TEST_CONSTANT(ATOMIC_CHAR32_T_LOCK_FREE, char32_t);
  TEST_CONSTANT(ATOMIC_WCHAR_T_LOCK_FREE, wchar_t);
  TEST_CONSTANT(ATOMIC_SHORT_LOCK_FREE, short);
  TEST_CONSTANT(ATOMIC_INT_LOCK_FREE, int);
  TEST_CONSTANT(ATOMIC_LONG_LOCK_FREE, long);
  TEST_CONSTANT(ATOMIC_LLONG_LOCK_FREE, long long);
  TEST_CONSTANT(ATOMIC_POINTER_LOCK_FREE, void *);
#undef TEST_CONSTANT

  exit(0);
}
