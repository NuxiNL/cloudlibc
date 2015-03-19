// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#ifndef COMMON_FLOAT16_H
#define COMMON_FLOAT16_H

#include <assert.h>
#include <fenv.h>
#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

// Converter for base-16 floating point literals to native types.
//
// This structure and functions below can be used to convert base-16
// string representations of floating point numbers to the native
// floating point types of the system.
//
// The following example shows how the string "0x123.cafep0" can be
// converted to a float:
//
//   struct f16enc f16;
//   f16enc_init(&f16);
//   f16enc_push_xdigit(&f16, 1); // '1'
//   f16enc_push_xdigit(&f16, 2); // '2'
//   f16enc_push_xdigit(&f16, 3); // '3'
//   f16enc_push_xdigit(&f16, 12); // 'c'
//   f16enc_push_xdigit(&f16, 10); // 'a'
//   f16enc_push_xdigit(&f16, 15); // 'f'
//   f16enc_push_xdigit(&f16, 14); // 'e'
//   bool have_range_error;
//   float f = f16enc_get_float(&f16, -16, FE_TONEAREST, &have_range_error);
//
// The encoder does not support the concept of a radix character, as it
// can simply be emulated by adjusting the exponent.

typedef uint64_t f16_significand_t;
#define F16_SIGNIFICAND_BITS 64

struct f16enc {
  f16_significand_t significand;  // Significand bits.
  uint8_t trailing;               // Trailing significand bits for rounding.
  int bits;                       // Total number of bits inserted.
  int shift;                      // Shift state for significand.
};

static inline void f16enc_init(struct f16enc *f16) {
  f16->significand = 0;
  f16->trailing = 0;
  f16->bits = -1;
  f16->shift = F16_SIGNIFICAND_BITS - 4;
}

static inline void f16enc_push_xdigit(struct f16enc *f16, uint8_t xdigit) {
  assert(xdigit < 16 && "Digit out of bounds");
  if (f16->shift >= 0) {
    // Add digit to the significand.
    f16->significand |= (f16_significand_t)xdigit << f16->shift;

    // First digit needs to be aligned so that the top bit of the
    // significand is set.
    while (f16->significand >> (F16_SIGNIFICAND_BITS - 1) == 0) {
      // Leading zero digit. Leave conversion in initial state.
      if (f16->significand == 0)
        return;
      f16->significand <<= 1;
      --f16->bits;
      ++f16->shift;
    }
    f16->shift -= 4;
  } else if (f16->shift >= -4) {
    // Set the last couple of bits of the significand. Also store the
    // other bits that don't fit anymore, as they will be used for the
    // rounding.
    f16->significand |= xdigit >> -f16->shift;
    f16->trailing |= xdigit << (8 + f16->shift);
    f16->shift -= 4;
  } else {
    // Garbage bits at the very end. If we try to parse a very long
    // number like 0x1.000.....0001 and have to round up, we must
    // not discard any input.
    f16->trailing |= xdigit << (8 + f16->shift);
  }
  f16->bits += 4;
}

// Returns floating zero in case the converter stores value zero.
#define RETURN_ZERO_IF_ZERO()                                     \
  do {                                                            \
    if (f16->significand == 0) {                                  \
      assert(f16->bits == -1 && "Invalid bit count");             \
      assert(f16->trailing == 0 && "Invalid trailing bits");      \
      *have_range_error = false;                                  \
      return 0.0;                                                 \
    }                                                             \
    assert(f16->significand >> (F16_SIGNIFICAND_BITS - 1) != 0 && \
           "Floating point value not normalized");                \
  } while (0)

// Adjusts the exponent for the number of bits of data we read. Adds the
// exponent bias as well. The exponent shall then lie between
// [1, 2^k - 2], where k is the number of exponent bits.
#define APPLY_BIAS_TO_EXPONENT(exp_dig, mant_dig, has_subnorm) \
  do {                                                         \
    int bias = (1 << (exp_dig - 1)) - 1;                       \
    exponent += f16->bits + bias;                              \
    if (exponent > (1 << exp_dig) - 2) {                       \
      /* Overflow. */                                          \
      *have_range_error = true;                                \
      return INFINITY;                                         \
    } else if (exponent <= 0) {                                \
      if (has_subnorm == 1 && exponent > 1 - mant_dig) {       \
        /* Number is still in subnormal range. */              \
        /* TODO(edje): Implement rounding! */                  \
        significand >>= 1 - exponent;                          \
        exponent = 0;                                          \
      } else {                                                 \
        /* Smaller than subnormal. */                          \
        *have_range_error = true;                              \
        return 0.0;                                            \
      }                                                        \
    }                                                          \
  } while (0)

// IEEE 754-2008 "bin32": Single-precision floating-point.

#define F16_BIN32_MANT_DIG 24

#if FLT_MANT_DIG == F16_BIN32_MANT_DIG
typedef float f16_bin32_t;
#define F16_BIN32_HAS_SUBNORM FLT_HAS_SUBNORM
#elif DBL_MANT_DIG == F16_BIN32_MANT_DIG
typedef double f16_bin32_t;
#define F16_BIN32_HAS_SUBNORM DBL_HAS_SUBNORM
#elif LDBL_MANT_DIG == F16_BIN32_MANT_DIG
typedef long double f16_bin32_t;
#define F16_BIN32_HAS_SUBNORM LDBL_HAS_SUBNORM
#else
#define F16_NO_BIN32
#endif

#ifndef F16_NO_BIN32
static inline f16_bin32_t f16enc_get_bin32(const struct f16enc *f16,
                                           int exponent, int round,
                                           bool *have_range_error) {
  RETURN_ZERO_IF_ZERO();

  // Apply rounding. For rounding to the nearest, we should only inspect
  // the first bit after the significand. When rounding upward, any bits
  // will do.
  uint32_t significand =
      f16->significand >> (F16_SIGNIFICAND_BITS - F16_BIN32_MANT_DIG);
  f16_significand_t remainder =
      f16->significand << F16_BIN32_MANT_DIG | f16->trailing;
  if ((round == FE_TONEAREST && remainder >> (F16_SIGNIFICAND_BITS - 1) != 0) ||
      (round == FE_UPWARD && remainder != 0)) {
    ++significand;
    if (significand >> F16_BIN32_MANT_DIG != 0) {
      significand >>= 1;
      ++exponent;
    }
  }

  APPLY_BIAS_TO_EXPONENT(8, F16_BIN32_MANT_DIG, F16_BIN32_HAS_SUBNORM);

  // Convert significand and exponent to native floating point type.
  union {
    uint32_t i;
    f16_bin32_t f;
  } result = {
      .i = (significand & ((UINT32_C(1) << (F16_BIN32_MANT_DIG - 1)) - 1)) |
           ((uint32_t)exponent << (F16_BIN32_MANT_DIG - 1))};
  static_assert(sizeof(result.i) == sizeof(result.f), "Size mismatch");
  *have_range_error = false;
  return result.f;
}
#endif

// IEEE 754-2008 "bin64": Double-precision floating-point.

#define F16_BIN64_MANT_DIG 53

#if FLT_MANT_DIG == F16_BIN64_MANT_DIG
typedef float f16_bin64_t;
#define F16_BIN64_HAS_SUBNORM FLT_HAS_SUBNORM
#elif DBL_MANT_DIG == F16_BIN64_MANT_DIG
typedef double f16_bin64_t;
#define F16_BIN64_HAS_SUBNORM DBL_HAS_SUBNORM
#elif LDBL_MANT_DIG == F16_BIN64_MANT_DIG
typedef long double f16_bin64_t;
#define F16_BIN64_HAS_SUBNORM LDBL_HAS_SUBNORM
#else
#define F16_NO_BIN64
#endif

#ifndef F16_NO_BIN64
static inline f16_bin64_t f16enc_get_bin64(const struct f16enc *f16,
                                           int exponent, int round,
                                           bool *have_range_error) {
  RETURN_ZERO_IF_ZERO();

  // Apply rounding. For rounding to the nearest, we should only inspect
  // the first bit after the significand. When rounding upward, any bits
  // will do.
  uint64_t significand =
      f16->significand >> (F16_SIGNIFICAND_BITS - F16_BIN64_MANT_DIG);
  f16_significand_t remainder =
      f16->significand << F16_BIN64_MANT_DIG | f16->trailing;
  if ((round == FE_TONEAREST && remainder >> (F16_SIGNIFICAND_BITS - 1) != 0) ||
      (round == FE_UPWARD && remainder != 0)) {
    ++significand;
    if (significand >> F16_BIN64_MANT_DIG != 0) {
      significand >>= 1;
      ++exponent;
    }
  }

  APPLY_BIAS_TO_EXPONENT(11, F16_BIN64_MANT_DIG, F16_BIN64_HAS_SUBNORM);

  // Convert significand and exponent to native floating point type.
  union {
    uint64_t i;
    f16_bin64_t f;
  } result = {
      .i = (significand & ((UINT64_C(1) << (F16_BIN64_MANT_DIG - 1)) - 1)) |
           ((uint64_t)exponent << (F16_BIN64_MANT_DIG - 1))};
  static_assert(sizeof(result.i) == sizeof(result.f), "Size mismatch");
  *have_range_error = false;
  return result.f;
}
#endif

// x86 80-bit Extended Precision Format.

#define F16_BIN80_MANT_DIG 64

#if FLT_MANT_DIG == F16_BIN80_MANT_DIG
typedef float f16_bin80_t;
#define F16_BIN80_HAS_SUBNORM FLT_HAS_SUBNORM
#elif DBL_MANT_DIG == F16_BIN80_MANT_DIG
typedef double f16_bin80_t;
#define F16_BIN80_HAS_SUBNORM DBL_HAS_SUBNORM
#elif LDBL_MANT_DIG == F16_BIN80_MANT_DIG
typedef long double f16_bin80_t;
#define F16_BIN80_HAS_SUBNORM LDBL_HAS_SUBNORM
#else
#define F16_NO_BIN80
#endif

#ifndef F16_NO_BIN80
static inline f16_bin80_t f16enc_get_bin80(const struct f16enc *f16,
                                           int exponent, int round,
                                           bool *have_range_error) {
  RETURN_ZERO_IF_ZERO();

  // Apply rounding. For rounding to the nearest, we should only inspect
  // the first bit after the significand. When rounding upward, any bits
  // will do.
  uint64_t significand = f16->significand;
  if ((round == FE_TONEAREST && f16->trailing >> 7 != 0) ||
      (round == FE_UPWARD && f16->trailing != 0)) {
    if (significand == UINT64_MAX) {
      significand = UINT64_C(1) << 63;
      ++exponent;
    } else {
      ++significand;
    }
  }

  APPLY_BIAS_TO_EXPONENT(15, F16_BIN80_MANT_DIG, F16_BIN80_HAS_SUBNORM);

  // Convert significand and exponent to native floating point type.
  union {
    uint64_t i[2];
    f16_bin80_t f;
  } result = {.i = {significand, exponent}};
  static_assert(sizeof(result.i) == sizeof(result.f), "Size mismatch");
  *have_range_error = false;
  return result.f;
}
#endif

#undef RETURN_ZERO_IF_ZERO
#undef APPLY_BIAS_TO_EXPONENT

static inline float f16enc_get_float(const struct f16enc *f16, int exponent,
                                     int round, bool *have_range_error) {
#if FLT_MANT_DIG == F16_BIN32_MANT_DIG
  return f16enc_get_bin32(f16, exponent, round, have_range_error);
#elif FLT_MANT_DIG == F16_BIN64_MANT_DIG
  return f16enc_get_bin64(f16, exponent, round, have_range_error);
#elif FLT_MANT_DIG == F16_BIN80_MANT_DIG
  return f16enc_get_bin80(f16, exponent, round, have_range_error);
#else
#error "Unsupported format"
#endif
}

static inline double f16enc_get_double(const struct f16enc *f16, int exponent,
                                       int round, bool *have_range_error) {
#if DBL_MANT_DIG == F16_BIN32_MANT_DIG
  return f16enc_get_bin32(f16, exponent, round, have_range_error);
#elif DBL_MANT_DIG == F16_BIN64_MANT_DIG
  return f16enc_get_bin64(f16, exponent, round, have_range_error);
#elif DBL_MANT_DIG == F16_BIN80_MANT_DIG
  return f16enc_get_bin80(f16, exponent, round, have_range_error);
#else
#error "Unsupported format"
#endif
}

static inline long double f16enc_get_long_double(const struct f16enc *f16,
                                                 int exponent, int round,
                                                 bool *have_range_error) {
#if LDBL_MANT_DIG == F16_BIN32_MANT_DIG
  return f16enc_get_bin32(f16, exponent, round, have_range_error);
#elif LDBL_MANT_DIG == F16_BIN64_MANT_DIG
  return f16enc_get_bin64(f16, exponent, round, have_range_error);
#elif LDBL_MANT_DIG == F16_BIN80_MANT_DIG
  return f16enc_get_bin80(f16, exponent, round, have_range_error);
#else
#error "Unsupported format"
#endif
}

#endif
