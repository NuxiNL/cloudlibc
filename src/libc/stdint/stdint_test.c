// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <assert.h>
#include <stdint.h>
#include <wchar.h>

static_assert(INT8_MIN == INT8_C(-0x80), "Value mismatch");
static_assert(INT16_MIN == INT16_C(-0x8000), "Value mismatch");
static_assert(INT32_MIN == INT32_C(-0x80000000), "Value mismatch");
static_assert(INT64_MIN == INT64_C(-0x8000000000000000), "Value mismatch");

static_assert(INT8_MAX == INT8_C(0x7f), "Value mismatch");
static_assert(INT16_MAX == INT16_C(0x7fff), "Value mismatch");
static_assert(INT32_MAX == INT32_C(0x7fffffff), "Value mismatch");
static_assert(INT64_MAX == INT64_C(0x7fffffffffffffff), "Value mismatch");

static_assert(UINT8_MAX == UINT8_C(0xff), "Value mismatch");
static_assert(UINT16_MAX == UINT16_C(0xffff), "Value mismatch");
static_assert(UINT32_MAX == UINT32_C(0xffffffff), "Value mismatch");
static_assert(UINT64_MAX == UINT64_C(0xffffffffffffffff), "Value mismatch");

static_assert(INT_LEAST8_MIN <= INT8_C(-0x7f), "Value mismatch");
static_assert(INT_LEAST16_MIN <= INT16_C(-0x7fff), "Value mismatch");
static_assert(INT_LEAST24_MIN <= INT24_C(-0x7fffff), "Value mismatch");
static_assert(INT_LEAST32_MIN <= INT32_C(-0x7fffffff), "Value mismatch");
static_assert(INT_LEAST40_MIN <= INT40_C(-0x7fffffffff), "Value mismatch");
static_assert(INT_LEAST48_MIN <= INT48_C(-0x7fffffffffff), "Value mismatch");
static_assert(INT_LEAST56_MIN <= INT56_C(-0x7fffffffffffff), "Value mismatch");
static_assert(INT_LEAST64_MIN <= INT64_C(-0x7fffffffffffffff),
              "Value mismatch");

static_assert(INT_LEAST8_MAX >= INT8_C(0x7f), "Value mismatch");
static_assert(INT_LEAST16_MAX >= INT16_C(0x7fff), "Value mismatch");
static_assert(INT_LEAST24_MAX >= INT24_C(0x7fffff), "Value mismatch");
static_assert(INT_LEAST32_MAX >= INT32_C(0x7fffffff), "Value mismatch");
static_assert(INT_LEAST40_MAX >= INT40_C(0x7fffffffff), "Value mismatch");
static_assert(INT_LEAST48_MAX >= INT48_C(0x7fffffffffff), "Value mismatch");
static_assert(INT_LEAST56_MAX >= INT56_C(0x7fffffffffffff), "Value mismatch");
static_assert(INT_LEAST64_MAX >= INT64_C(0x7fffffffffffffff), "Value mismatch");

static_assert(UINT_LEAST8_MAX >= UINT8_C(0xff), "Value mismatch");
static_assert(UINT_LEAST16_MAX >= UINT16_C(0xffff), "Value mismatch");
static_assert(UINT_LEAST24_MAX >= UINT24_C(0xffffff), "Value mismatch");
static_assert(UINT_LEAST32_MAX >= UINT32_C(0xffffffff), "Value mismatch");
static_assert(UINT_LEAST40_MAX >= UINT40_C(0xffffffffff), "Value mismatch");
static_assert(UINT_LEAST48_MAX >= UINT48_C(0xffffffffffff), "Value mismatch");
static_assert(UINT_LEAST56_MAX >= UINT56_C(0xffffffffffffff), "Value mismatch");
static_assert(UINT_LEAST64_MAX >= UINT64_C(0xffffffffffffffff),
              "Value mismatch");

static_assert(INT_FAST8_MIN <= INT8_C(-0x7f), "Value mismatch");
static_assert(INT_FAST16_MIN <= INT16_C(-0x7fff), "Value mismatch");
static_assert(INT_FAST24_MIN <= INT24_C(-0x7fffff), "Value mismatch");
static_assert(INT_FAST32_MIN <= INT32_C(-0x7fffffff), "Value mismatch");
static_assert(INT_FAST40_MIN <= INT40_C(-0x7fffffffff), "Value mismatch");
static_assert(INT_FAST48_MIN <= INT48_C(-0x7fffffffffff), "Value mismatch");
static_assert(INT_FAST56_MIN <= INT56_C(-0x7fffffffffffff), "Value mismatch");
static_assert(INT_FAST64_MIN <= INT64_C(-0x7fffffffffffffff), "Value mismatch");

static_assert(INT_FAST8_MAX >= INT8_C(0x7f), "Value mismatch");
static_assert(INT_FAST16_MAX >= INT16_C(0x7fff), "Value mismatch");
static_assert(INT_FAST24_MAX >= INT24_C(0x7fffff), "Value mismatch");
static_assert(INT_FAST32_MAX >= INT32_C(0x7fffffff), "Value mismatch");
static_assert(INT_FAST40_MAX >= INT40_C(0x7fffffffff), "Value mismatch");
static_assert(INT_FAST48_MAX >= INT48_C(0x7fffffffffff), "Value mismatch");
static_assert(INT_FAST56_MAX >= INT56_C(0x7fffffffffffff), "Value mismatch");
static_assert(INT_FAST64_MAX >= INT64_C(0x7fffffffffffffff), "Value mismatch");

static_assert(UINT_FAST8_MAX >= UINT8_C(0xff), "Value mismatch");
static_assert(UINT_FAST16_MAX >= UINT16_C(0xffff), "Value mismatch");
static_assert(UINT_FAST24_MAX >= UINT24_C(0xffffff), "Value mismatch");
static_assert(UINT_FAST32_MAX >= UINT32_C(0xffffffff), "Value mismatch");
static_assert(UINT_FAST40_MAX >= UINT40_C(0xffffffffff), "Value mismatch");
static_assert(UINT_FAST48_MAX >= UINT48_C(0xffffffffffff), "Value mismatch");
static_assert(UINT_FAST56_MAX >= UINT56_C(0xffffffffffffff), "Value mismatch");
static_assert(UINT_FAST64_MAX >= UINT64_C(0xffffffffffffffff),
              "Value mismatch");

static_assert(INTPTR_MIN <= -0x7fff, "Limit too low");
static_assert(INTPTR_MAX >= 0x7fff, "Limit too low");
static_assert(UINTPTR_MAX >= 0xffff, "Limit too low");

static_assert(INTMAX_MIN <= INTMAX_C(-0x7fffffffffffffff), "Limit too low");
static_assert(INTMAX_MAX >= INTMAX_C(0x7fffffffffffffff), "Limit too low");
static_assert(UINTMAX_MAX >= UINTMAX_C(0xffffffffffffffff), "Limit too low");

static_assert(PTRDIFF_MIN <= -0xffff, "Limit too low");
static_assert(PTRDIFF_MAX >= 0xffff, "Limit too low");

static_assert(SIZE_MAX >= 0xffff, "Limit too low");

static_assert((wchar_t)-1 < 0 ? WCHAR_MIN <= -0x7f : WCHAR_MIN == 0,
              "Limit too low");
static_assert((wchar_t)-1 < 0 ? WCHAR_MAX >= 0x7f : WCHAR_MAX >= 0xff,
              "Limit too low");

static_assert((wint_t)-1 < 0 ? WINT_MIN <= -0x7fff : WINT_MIN == 0,
              "Limit too low");
static_assert((wint_t)-1 < 0 ? WINT_MAX >= 0x7fff : WINT_MAX >= 0xffff,
              "Limit too low");
