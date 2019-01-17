// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <assert.h>
#include <complex.h>
#include <program.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define ASSERT_EQ(a, b) assert((a) == (b))
#define ASSERT_TRUE(a) assert(a)

// Test the logical behaviour of operations on atomic integer types.
// These tests make no attempt to actually test whether the functions
// are atomic or provide the right barrier semantics.
//
// For every type, we create an array of 16 elements and repeat the test
// on every element in the array. This allows us to test whether the
// atomic operations have no effect on surrounding values. This is
// especially useful for the smaller integer types, as it may be the
// case that these operations are implemented by processing entire words
// (e.g. on MIPS).

#define DO_FETCH_TEST(T, Tdelta, a, name, result) \
  do {                                            \
    T v1 = atomic_load(a);                        \
    Tdelta v2;                                    \
    arc4random_buf(&v2, sizeof(T));               \
    T v3 = atomic_##name(a, v2);                  \
    ASSERT_EQ(0, memcmp(&v1, &v3, sizeof(T)));    \
    T v4 = result;                                \
    T v5 = atomic_load(a);                        \
    ASSERT_EQ(0, memcmp(&v4, &v5, sizeof(T)));    \
  } while (0)

#define DO_COMPARE_EXCHANGE_TEST(T, a, name)                   \
  do {                                                         \
    T v1 = atomic_load(a);                                     \
    T v2;                                                      \
    arc4random_buf(&v2, sizeof(T));                            \
    T v3;                                                      \
    arc4random_buf(&v3, sizeof(T));                            \
    if (atomic_compare_exchange_##name(a, &v2, v3))            \
      ASSERT_EQ(0, memcmp(&v2, &v1, sizeof(T)));               \
    else                                                       \
      ASSERT_TRUE(atomic_compare_exchange_strong(a, &v2, v3)); \
    T v4 = atomic_load(a);                                     \
    ASSERT_EQ(0, memcmp(&v3, &v4, sizeof(T)));                 \
  } while (0)

#define COMMON_TESTS(T, a)                       \
  do {                                           \
    {                                            \
      T v1;                                      \
      arc4random_buf(&v1, sizeof(T));            \
      atomic_init(a, v1);                        \
      T v2 = atomic_load(a);                     \
      ASSERT_EQ(0, memcmp(&v1, &v2, sizeof(T))); \
    }                                            \
    {                                            \
      T v1;                                      \
      arc4random_buf(&v1, sizeof(T));            \
      atomic_store(a, v1);                       \
      T v2 = atomic_load(a);                     \
      ASSERT_EQ(0, memcmp(&v1, &v2, sizeof(T))); \
    }                                            \
                                                 \
    DO_FETCH_TEST(T, T, a, exchange, v2);        \
    DO_COMPARE_EXCHANGE_TEST(T, a, weak);        \
    DO_COMPARE_EXCHANGE_TEST(T, a, strong);      \
  } while (0)

#define INTEGER_TESTS(T, a)                     \
  do {                                          \
    COMMON_TESTS(T, a);                         \
                                                \
    DO_FETCH_TEST(T, T, a, fetch_add, v1 + v2); \
    DO_FETCH_TEST(T, T, a, fetch_sub, v1 - v2); \
    DO_FETCH_TEST(T, T, a, fetch_and, v1 &v2);  \
    DO_FETCH_TEST(T, T, a, fetch_or, v1 | v2);  \
    DO_FETCH_TEST(T, T, a, fetch_xor, v1 ^ v2); \
  } while (0)

#define POINTER_TESTS(T, a)                             \
  do {                                                  \
    COMMON_TESTS(T, a);                                 \
                                                        \
    DO_FETCH_TEST(T, ptrdiff_t, a, fetch_add, v1 + v2); \
    DO_FETCH_TEST(T, ptrdiff_t, a, fetch_sub, v1 - v2); \
  } while (0)

#define TEST_TYPE(T, suite)                            \
  for (int i = 0; i < 100; ++i) {                      \
    struct {                                           \
      _Atomic(T) v[16];                                \
    } list, cmp;                                       \
    arc4random_buf(&cmp, sizeof(cmp));                 \
    for (int j = 0; j < 16; ++j) {                     \
      list = cmp;                                      \
      suite(T, &list.v[j]);                            \
      list.v[j] = cmp.v[j];                            \
      ASSERT_EQ(0, memcmp(&cmp, &list, sizeof(list))); \
    }                                                  \
  }

void program_main(const argdata_t *ad) {
  {
    TEST_TYPE(int8_t, INTEGER_TESTS);
    TEST_TYPE(uint8_t, INTEGER_TESTS);
    TEST_TYPE(int16_t, INTEGER_TESTS);
    TEST_TYPE(uint16_t, INTEGER_TESTS);
    TEST_TYPE(int32_t, INTEGER_TESTS);
    TEST_TYPE(uint32_t, INTEGER_TESTS);
    TEST_TYPE(int64_t, INTEGER_TESTS);
    TEST_TYPE(uint64_t, INTEGER_TESTS);
  }

  {
    TEST_TYPE(float, COMMON_TESTS);
    TEST_TYPE(double, COMMON_TESTS);
#if 0  // TODO(ed): Clang bug: wrong intrinsics functions.
  TEST_TYPE(long double, COMMON_TESTS);
#endif
  }

  {
    TEST_TYPE(float complex, COMMON_TESTS);
#if 0  // TODO(ed): Clang bug: wrong intrinsics functions.
  TEST_TYPE(double complex, COMMON_TESTS);
#endif
#if 0  // TODO(ed): Clang bug: causes Clang to crash.
  TEST_TYPE(long double complex, COMMON_TESTS);
#endif
  }

  {
    struct somestruct {
      char data[48];
    };

    TEST_TYPE(struct somestruct, COMMON_TESTS);
  }

  {
    struct somestruct {
      char data[48];
    };

    TEST_TYPE(int8_t *, POINTER_TESTS);
    TEST_TYPE(int16_t *, POINTER_TESTS);
    TEST_TYPE(int32_t *, POINTER_TESTS);
    TEST_TYPE(int64_t *, POINTER_TESTS);
    TEST_TYPE(float *, POINTER_TESTS);
    TEST_TYPE(double *, POINTER_TESTS);
    TEST_TYPE(long double *, POINTER_TESTS);
    TEST_TYPE(float complex *, POINTER_TESTS);
    TEST_TYPE(double complex *, POINTER_TESTS);
    TEST_TYPE(long double complex *, POINTER_TESTS);
    TEST_TYPE(struct somestruct *, POINTER_TESTS);
  }

  exit(0);
}
