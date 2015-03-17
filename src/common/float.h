// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#ifndef COMMON_FLOAT_H
#define COMMON_FLOAT_H

#include <locale.h>
#include <stdbool.h>
#include <stdint.h>
#include <wchar.h>

// A base-2 floating point number. The value associated with a number
// in this structure is equal to:
//
//     (significand[0] << 64 | significand[1]) * 2^exponent
struct float2 {
  uint64_t significand[2];
  int exponent;
  int flags;
#define F2_INFINITE 0x1
#define F2_NEGATIVE 0x2
#define F2_NAN 0x4
};

__BEGIN_DECLS
void __float2_normalize(struct float2 *);
bool __float2_to_float(const struct float2 *, float *);
bool __float2_to_double(const struct float2 *, double *);
bool __float2_to_long_double(const struct float2 *, long double *);
void __long_double_to_float2(long double, struct float2 *);
__END_DECLS

// Handling of base-10 floating point numbers. These functions are
// wrappers around the double-conversion library.

__BEGIN_DECLS
float __float10_to_float(const char *, size_t, int);
double __float10_to_double(const char *, size_t, int);
__END_DECLS

__BEGIN_DECLS
bool __strtofloat2(const char *__restrict, char **__restrict, locale_t,
                   struct float2 *);
bool __wcstofloat2(const wchar_t *__restrict, wchar_t **__restrict, locale_t,
                   struct float2 *);
__END_DECLS

#endif
