// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#ifndef COMMON_FLOAT16_H
#define COMMON_FLOAT16_H

#include <common/overflow.h>

#include <assert.h>
#include <fenv.h>
#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
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

typedef uint64_t f16_part_t;
#define F16_PART_BITS 64
#define F16_NPARTS 2

// Creats a bitmask with the n'th bit set.
static inline f16_part_t f16_bit(unsigned int n) {
  return (f16_part_t)1 << (F16_PART_BITS - 1) >> (n % F16_PART_BITS);
}

// Returns whether one of the 'amount' last bits of the significand is
// non-zero.
static inline bool f16_has_trailing_bits(const f16_part_t *parts,
                                         unsigned int amount) {
  // Test complete parts.
  for (unsigned int i = F16_NPARTS - amount / F16_PART_BITS; i < F16_NPARTS;
       ++i)
    if (parts[i] != 0)
      return true;
  // Test bits within a part.
  return (parts[F16_NPARTS - amount / F16_PART_BITS - 1] &
          (((f16_part_t)1 << (amount % F16_PART_BITS)) - 1)) != 0;
}

// Determines whether the significand is all zero.
static inline bool f16_is_zero(const f16_part_t *parts) {
  for (unsigned int i = 0; i < F16_NPARTS; i++)
    if (parts[i] != 0)
      return false;
  return true;
}

// Shifts a significand to the left.
static inline void f16_shift_left(f16_part_t *parts, unsigned int amount) {
  assert(amount < F16_PART_BITS && "Shifting more than one part");
  unsigned int i = 0;
  for (;;) {
    parts[i] <<= amount;
    if (i == F16_NPARTS - 1)
      return;
    parts[i] |= parts[i + 1] >> (F16_PART_BITS - amount);
    ++i;
  }
}

// Shifts a significand to the right, while preserving the final bits
// for rounding.
static inline void f16_shift_right(f16_part_t *parts, unsigned int amount) {
  // Keep track of whether one of the bits we're going to discard is
  // non-zero. If that's the case, we set the very last bit to 1, so
  // that rounding works all right.
  bool trailing_bits = f16_has_trailing_bits(parts, amount);

  // Shift entire significand right by the specified number of bits.
  unsigned int idx = amount / F16_PART_BITS;
  unsigned int shift = amount % F16_PART_BITS;
  unsigned int i = F16_NPARTS - 1;
  for (;;) {
    f16_part_t right = i >= idx ? parts[i - idx] : 0;
    if (shift == 0) {
      // Shift by an exact number of parts.
      parts[i] = right;
    } else {
      // Shift by a number of bits that is not a multiple of the parts
      // width. Merge successive parts together.
      f16_part_t left = i >= idx + 1 ? parts[i - idx - 1] : 0;
      left <<= 64 - shift;
      right >>= shift;
      parts[i] = left | right;
    }
    if (i-- == 0)
      break;
  }

  // Set trailing bit to make rounding work.
  if (trailing_bits)
    parts[F16_NPARTS - 1] |= 0x1;
}

// Rounds a significand to the width of the resulting floating point
// type. The rounding mode is provided in the form of a <fenv.h> mode.
static inline void f16_apply_rounding(f16_part_t *parts, int *exponent,
                                      int round, unsigned int mant_dig) {
  // We should round the number upwards in two cases, namely when
  // performing nearest neighbour rounding and the top bit of the
  // truncated part of the significand is set, or when performing upward
  // rounding and any of the bits in the truncated part is set.
  if ((round == FE_TONEAREST &&
       (parts[mant_dig / F16_PART_BITS] & (f16_bit(mant_dig))) != 0) ||
      (round == FE_UPWARD &&
       f16_has_trailing_bits(parts, F16_PART_BITS * F16_NPARTS - mant_dig))) {
    // Increment the retained part of the significand by one.
    unsigned int i = --mant_dig / F16_PART_BITS;
    f16_part_t inc = f16_bit(mant_dig);
    for (;;) {
      if (!add_overflow(parts[i], inc, &parts[i]))
        return;
      if (i-- == 0) {
        // Increment overflowed the significand. Increment the exponent.
        parts[0] = f16_bit(0);
        ++*exponent;
        return;
      }
      // Increment overflowed a part of the significand. Continue
      // incrementing the parts that lead up to it.
      inc = 1;
    }
  }
}

struct f16enc {
  f16_part_t parts[F16_NPARTS];  // Parts of the significand.
  unsigned int bits;             // Total number of bits inserted.
};

// Initializes an encoder object.
static inline void f16enc_init(struct f16enc *f16) {
  for (size_t i = 0; i < F16_NPARTS; ++i)
    f16->parts[i] = 0;
  f16->bits = 0;
}

// Pushes a hexadecimal digit into the encoder state.
static inline void f16enc_push_xdigit(struct f16enc *f16, uint8_t xdigit) {
  assert(xdigit < 16 && "Digit out of bounds");
  if (f16->bits == 0) {
    // First digit. Discard leading zeroes.
    if (xdigit == 0)
      return;
    // Shift character until the top bit is set. This ensures that the
    // resulting floating point value is normalized.
    f16->parts[0] |= (f16_part_t)xdigit << (F16_PART_BITS - 4);
    while ((f16->parts[0] & f16_bit(0)) == 0) {
      f16->parts[0] <<= 1;
      --f16->bits;
    }
  } else if (f16->bits >= F16_PART_BITS * F16_NPARTS - 4) {
    // We're pushing more digits than we can store internally. If we try
    // to parse a very long number like 0x1.000.....0001 and have to
    // round up, we must not discard any input.
    f16->parts[F16_NPARTS - 1] |= xdigit;
  } else {
    // Digit somewhere in the middle. Store the bits in the right place.
    unsigned int idx = f16->bits / F16_PART_BITS;
    unsigned int shift = f16->bits % F16_PART_BITS;
    f16->parts[idx] |= (f16_part_t)xdigit << (F16_PART_BITS - 4) >> shift;
    // Digit is stored at the very end of the part. Let bits that don't
    // fit trickle into the next part.
    if (shift > F16_PART_BITS - 4)
      f16->parts[idx + 1] = (f16_part_t)xdigit
                            << (F16_PART_BITS * 2 - 4 - shift);
  }
  f16->bits += 4;
}

// Common code for f16enc_get_bin*().
#define F16ENC_GET_BIN(exp_dig, mant_dig, has_subnorm)                \
  f16_part_t parts[F16_NPARTS];                                       \
  do {                                                                \
    for (size_t i = 0; i < F16_NPARTS; ++i)                           \
      parts[i] = f16->parts[i];                                       \
    if (parts[0] == 0) {                                              \
      /* Floating point value zero. */                                \
      assert(f16->bits == 0 && "Invalid bit count");                  \
      *have_range_error = false;                                      \
      return 0.0;                                                     \
    }                                                                 \
    assert((parts[0] & f16_bit(0)) != 0 &&                            \
           "Floating point value not normalized");                    \
                                                                      \
    /* Add correction for input length and bias to the exponent. */   \
    exponent += f16->bits + (1 << (exp_dig - 1)) - 2;                 \
    static const int max_exp = (1 << exp_dig) - 2;                    \
    if (exponent > max_exp) {                                         \
      /* Overflow. */                                                 \
      *have_range_error = true;                                       \
      return INFINITY;                                                \
    } else if (exponent <= 0) {                                       \
      if (has_subnorm && exponent > 1 - mant_dig) {                   \
        /* Number is in subnormal range. */                           \
        f16_shift_right(parts, 1 - exponent);                         \
        exponent = 1;                                                 \
      } else {                                                        \
        /* Smaller than subnormal. */                                 \
        *have_range_error = true;                                     \
        return 0.0;                                                   \
      }                                                               \
    }                                                                 \
                                                                      \
    /* Apply rounding. This may cause an overflow once more. */       \
    f16_apply_rounding(parts, &exponent, round, mant_dig);            \
    if (exponent > max_exp) {                                         \
      /* Overflow. */                                                 \
      *have_range_error = true;                                       \
      return INFINITY;                                                \
    }                                                                 \
                                                                      \
    /* Quirk: subnormals use an exponent of zero instead of 1. */     \
    if (has_subnorm && exponent == 1 && (parts[0] & f16_bit(0)) == 0) \
      exponent = 0;                                                   \
                                                                      \
    /* Going to return a valid floating point value. */               \
    *have_range_error = false;                                        \
  } while (0)

// IEEE 754-2008 "binary32": Single-precision floating-point.

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
  F16ENC_GET_BIN(8, F16_BIN32_MANT_DIG, F16_BIN32_HAS_SUBNORM);

  // Convert significand and exponent to native floating point type.
  union {
    uint32_t i;
    f16_bin32_t f;
  } result = {.i = (uint32_t)exponent << (F16_BIN32_MANT_DIG - 1) |
                   parts[0] << 1 >> (1 + F16_PART_BITS - F16_BIN32_MANT_DIG)};
  static_assert(sizeof(result.i) == sizeof(result.f), "Size mismatch");
  return result.f;
}
#endif

// IEEE 754-2008 "binary64": Double-precision floating-point.

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
  F16ENC_GET_BIN(11, F16_BIN64_MANT_DIG, F16_BIN64_HAS_SUBNORM);

  // Convert significand and exponent to native floating point type.
  union {
    uint64_t i;
    f16_bin64_t f;
  } result = {.i = (uint64_t)exponent << (F16_BIN64_MANT_DIG - 1) |
                   parts[0] << 1 >> (1 + F16_PART_BITS - F16_BIN64_MANT_DIG)};
  static_assert(sizeof(result.i) == sizeof(result.f), "Size mismatch");
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
  F16ENC_GET_BIN(15, F16_BIN80_MANT_DIG, F16_BIN80_HAS_SUBNORM);

  // Convert significand and exponent to native floating point type.
  union {
    struct {
      uint64_t significand;
#ifdef __i386__
      uint32_t exponent;
#else
      uint64_t exponent;
#endif
    } i;
    f16_bin80_t f;
  } result = {.i = {.significand = parts[0], .exponent = exponent}};
  static_assert(sizeof(result.i) == sizeof(result.f), "Size mismatch");
  return result.f;
}
#endif

// IEEE 754-2008 "binary128": Quadruple-precision floating-point.

#define F16_BIN128_MANT_DIG 113

#if FLT_MANT_DIG == F16_BIN128_MANT_DIG
typedef float f16_bin128_t;
#define F16_BIN128_HAS_SUBNORM FLT_HAS_SUBNORM
#elif DBL_MANT_DIG == F16_BIN128_MANT_DIG
typedef double f16_bin128_t;
#define F16_BIN128_HAS_SUBNORM DBL_HAS_SUBNORM
#elif LDBL_MANT_DIG == F16_BIN128_MANT_DIG
typedef long double f16_bin128_t;
#define F16_BIN128_HAS_SUBNORM LDBL_HAS_SUBNORM
#else
#define F16_NO_BIN128
#endif

#ifndef F16_NO_BIN128
static inline f16_bin128_t f16enc_get_bin128(const struct f16enc *f16,
                                             int exponent, int round,
                                             bool *have_range_error) {
  F16ENC_GET_BIN(15, F16_BIN128_MANT_DIG, F16_BIN128_HAS_SUBNORM);

  // Convert significand and exponent to native floating point type.
  union {
    uint64_t i[2];
    f16_bin128_t f;
  } result = {.i = {
                  parts[0] << 49 | parts[1] >> 15,
                  (uint64_t)exponent << 48 | (parts[0] & ~f16_bit(0)) >> 15,
              }};
  static_assert(sizeof(result.i) == sizeof(result.f), "Size mismatch");
  return result.f;
}
#endif

static inline float f16enc_get_float(const struct f16enc *f16, int exponent,
                                     int round, bool *have_range_error) {
#if FLT_MANT_DIG == F16_BIN32_MANT_DIG
  return f16enc_get_bin32(f16, exponent, round, have_range_error);
#elif FLT_MANT_DIG == F16_BIN64_MANT_DIG
  return f16enc_get_bin64(f16, exponent, round, have_range_error);
#elif FLT_MANT_DIG == F16_BIN80_MANT_DIG
  return f16enc_get_bin80(f16, exponent, round, have_range_error);
#elif FLT_MANT_DIG == F16_BIN128_MANT_DIG
  return f16enc_get_bin128(f16, exponent, round, have_range_error);
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
#elif DBL_MANT_DIG == F16_BIN128_MANT_DIG
  return f16enc_get_bin128(f16, exponent, round, have_range_error);
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
#elif LDBL_MANT_DIG == F16_BIN128_MANT_DIG
  return f16enc_get_bin128(f16, exponent, round, have_range_error);
#else
#error "Unsupported format"
#endif
}

// Converter for floating point native types to base-16 literals.
//
// This function can convert a normal or subnormal floating point value
// to a sequence of hexadecimal digits and extract its exponent. This
// can be used to implement printf()'s "%a".
//
// The following piece of code converts the floating point value 1.51 to
// a hexadecimal representation with a precision of two digits, rounding
// the number upward to 0x1.83p0:
//
//   unsigned char digits[2];
//   size_t ndigits = 2;
//   int exponent;
//   f16dec(1.51, digits, &ndigits, &exponent, FE_UPWARD);
//
// The digits array will now hold the numbers 8 and 3.
static inline void f16dec(long double f, unsigned char *digits, size_t *ndigits,
                          int *exponent, int round) {
  // Invert the rounding mode if the value is negative, so that the code
  // below does not need to take the sign bit into account.
  if (signbit(f)) {
    if (round == FE_UPWARD)
      round = FE_DOWNWARD;
    else if (round == FE_DOWNWARD)
      round = FE_UPWARD;
  }

#if LDBL_MANT_DIG == 53
  // Extract the significand and the exponent from the floating point value.
  union {
    long double f;
    uint64_t i;
  } value = {.f = f};
  static_assert(sizeof(value.f) == sizeof(value.i), "Size mismatch");
  f16_part_t parts[F16_NPARTS] = {value.i << 11};
  *exponent = (value.i >> 52) & 0x7ff;
#elif LDBL_MANT_DIG == 64
  union {
    long double f;
    struct {
      uint64_t significand;
#ifdef __i386__
      uint32_t exponent;
#else
      uint64_t exponent;
#endif
    } i;
  } value = {.f = f};
  static_assert(sizeof(value.f) == sizeof(value.i), "Size mismatch");
  f16_part_t parts[F16_NPARTS] = {value.i.significand};
  *exponent = value.i.exponent & 0x7fff;
#elif LDBL_MANT_DIG == 113
  union {
    long double f;
    uint64_t i[2];
  } value = {.f = f};
  static_assert(sizeof(value.f) == sizeof(value.i), "Size mismatch");
  f16_part_t parts[F16_NPARTS] = {
      value.i[1] << 15 | value.i[0] >> 49,
      value.i[0] << 15,
  };
  *exponent = (value.i[1] >> 48) & 0x7fff;
#else
#error "Unsupported format"
#endif

  if (*exponent == 0) {
    // Subnormal floating point value. Normalize it.
    assert(!f16_is_zero(parts) && "Floating point has value zero");
    *exponent = LDBL_MIN_EXP - 1;
    while ((parts[0] & f16_bit(0)) == 0) {
      f16_shift_left(parts, 1);
      --*exponent;
    }
  } else {
    // Normal floating point value.
    parts[0] |= f16_bit(0);
    *exponent += LDBL_MIN_EXP - 2;
  }

  // Apply rounding if the number of digits requested is less than the
  // size of the significand.
  size_t nbits = *ndigits * 4 + 1;
  if (nbits < F16_PART_BITS * F16_NPARTS)
    f16_apply_rounding(parts, exponent, round, nbits);

  // Convert bits after the radix to hexadecimal digits.
  f16_shift_left(parts, 1);
  for (size_t i = 0; i < *ndigits; ++i) {
    if (f16_is_zero(parts)) {
      *ndigits = i;
      break;
    }
    digits[i] = parts[0] >> (F16_PART_BITS - 4);
    f16_shift_left(parts, 4);
  }
}

#endif
