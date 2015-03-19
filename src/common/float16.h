// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#ifndef COMMON_F16_H
#define COMMON_F16_H

#include <assert.h>
#include <fenv.h>
#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

typedef uint64_t f16_significand_t;
#define F16_SIGNIFICAND_BITS 64
#define F16_SHIFT_INIT (F16_SIGNIFICAND_BITS - 4)

struct f16enc {
  f16_significand_t significand;
  uint8_t trailing;
  int bits;
  int shift;
};

static inline void f16enc_init(struct f16enc *f16) {
  f16->significand = 0;
  f16->trailing = 0;
  f16->bits = -1;
  f16->shift = F16_SHIFT_INIT;
}

static inline void f16enc_push_xdigit(struct f16enc *f16, uint8_t xdigit) {
  assert(xdigit < 16 && "Digit out of bounds");
  f16->bits += 4;
  if (f16->shift >= 0) {
    // Add digit to the significand.
    f16->significand |= (f16_significand_t)xdigit << f16->shift;

    // First digit needs to be aligned so that the top bit of the
    // significand is set.
    while (f16->significand >> (F16_SIGNIFICAND_BITS - 1) == 0) {
      // Leading zero digit. Leave conversion in initial state.
      if (f16->significand == 0) {
        f16->bits = -1;
        return;
      }
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
    // discard any input.
    f16->trailing |= xdigit << (8 + f16->shift);
  }
}

// IEEE 754-2008 "bin32": Single-precision floating-point.

#define F16_BIN32_MANT_DIG 24
#define F16_BIN32_EXP_DIG 8
#define F16_BIN32_EXP_BIAS 127

#if FLT_MANT_DIG == F16_BIN32_MANT_DIG
typedef float f16_bin32_t;
#elif DBL_MANT_DIG == F16_BIN32_MANT_DIG
typedef double f16_bin32_t;
#elif LDBL_MANT_DIG == F16_BIN32_MANT_DIG
typedef long double f16_bin32_t;
#else
#define F16_NO_BIN32
#endif

#ifndef F16_NO_BIN32
static inline f16_bin32_t f16enc_get_bin32(const struct f16enc *f16,
                                           int exponent, int round,
                                           bool *have_range_error) {
  // Zero value.
  if (f16->bits == 0) {
    assert(f16->bits == -1 && "Invalid bit count");
    assert(f16->trailing == 0 && "Invalid trailing bits");
    *have_range_error = false;
    return 0.0;
  }
  assert(f16->significand >> (F16_SIGNIFICAND_BITS - 1) != 0 &&
         "Floating point value not normalized");

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

  // Adjust the exponent for the number of bits of data we read. Add the
  // exponent bias as well. The exponent should now lie between
  // [1, 2^k - 2], where k is the number of exponent bits.
  exponent += f16->bits + F16_BIN32_EXP_BIAS;
  if (exponent > (1 << F16_BIN32_EXP_DIG) - 2) {
    // Overflow.
    *have_range_error = true;
    return INFINITY;
  } else if (exponent < 1) {
    // TODO(edje): Support subnormals!
    // Underflow.
    *have_range_error = true;
    return 0.0;
  }

  // Convert significand and exponent to native floating point type.
  union {
    uint32_t i;
    f16_bin32_t f;
  } result = {.i = (significand & ((1 << F16_BIN32_MANT_DIG) - 1)) |
                   (exponent << (F16_BIN32_MANT_DIG - 1))};
  *have_range_error = false;
  return result.f;
}
#endif

// IEEE 754-2008 "bin64": Double-precision floating-point.

#if FLT_MANT_DIG == 53
typedef float f16_bin64_t;
#elif DBL_MANT_DIG == 53
typedef double f16_bin64_t;
#elif LDBL_MANT_DIG == 53
typedef long double f16_bin64_t;
#else
#define F16_NO_BIN64
#endif

#ifndef F16_NO_BIN64
static inline f16_bin64_t f16enc_get_bin64(const struct f16enc *f16,
                                           int exponent, int round,
                                           bool *have_range_error) {
  // TODO(edje): Implement!
  *have_range_error = true;
  return 0.0;
}
#endif

// x86 80-bit Extended Precision Format.

#if FLT_MANT_DIG == 64
typedef float f16_bin80_t;
#elif DBL_MANT_DIG == 64
typedef double f16_bin80_t;
#elif LDBL_MANT_DIG == 64
typedef long double f16_bin80_t;
#else
#define F16_NO_BIN80
#endif

#ifndef F16_NO_BIN80
static inline f16_bin80_t f16enc_get_bin80(const struct f16enc *f16,
                                           int exponent, int round,
                                           bool *have_range_error) {
  // TODO(edje): Implement!
  *have_range_error = true;
  return 0.0;
}
#endif

static inline float f16enc_get_float(const struct f16enc *f16, int exponent,
                                     int round, bool *have_range_error) {
#if FLT_MANT_DIG == 24
  return f16enc_get_bin32(f16, exponent, round, have_range_error);
#elif FLT_MANT_DIG == 53
  return f16enc_get_bin64(f16, exponent, round, have_range_error);
#elif FLT_MANT_DIG == 64
  return f16enc_get_bin80(f16, exponent, round, have_range_error);
#else
#error "Unsupported format"
#endif
}

static inline double f16enc_get_double(const struct f16enc *f16, int exponent,
                                       int round, bool *have_range_error) {
#if DBL_MANT_DIG == 24
  return f16enc_get_bin32(f16, exponent, round, have_range_error);
#elif DBL_MANT_DIG == 53
  return f16enc_get_bin64(f16, exponent, round, have_range_error);
#elif DBL_MANT_DIG == 64
  return f16enc_get_bin80(f16, exponent, round, have_range_error);
#else
#error "Unsupported format"
#endif
}

static inline long double f16enc_get_long_double(const struct f16enc *f16,
                                                 int exponent, int round,
                                                 bool *have_range_error) {
#if LDBL_MANT_DIG == 24
  return f16enc_get_bin32(f16, exponent, round, have_range_error);
#elif LDBL_MANT_DIG == 53
  return f16enc_get_bin64(f16, exponent, round, have_range_error);
#elif LDBL_MANT_DIG == 64
  return f16enc_get_bin80(f16, exponent, round, have_range_error);
#else
#error "Unsupported format"
#endif
}

#endif
