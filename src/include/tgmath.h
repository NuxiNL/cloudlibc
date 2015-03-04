// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.

// <tgmath.h> - type-generic macros

#ifndef _TGMATH_H_
#define _TGMATH_H_

#include <complex.h>
#include <math.h>

// Expands to a different expression based on the type of x.
// clang-format off
#define __tg_generic(x, f, d, ld, fc, dc, ldc) \
  _Generic(x,                                  \
           float: f,                           \
           default: d,                         \
           long double: ld,                    \
           float complex: fc,                  \
           double complex: dc,                 \
           long double complex: ldc)
// clang-format on

// Normalizes an expression to an expression of a floating point type.
// This is needed to ensure that expressions are properly type promoted
// under addition.
#define __tg_type(x)                                                     \
  __tg_generic(x, (float)0, (double)0, (long double)0, (float complex)0, \
               (double complex)0, (long double complex)0)

// Expands to a different floating point function based on the type of
// x, y and z.
#define __tg_f1(x, fn, cfn) \
  __tg_generic(x, fn##f, fn, fn##l, cfn##f, cfn, cfn##l)
#define __tg_f2(x, y, fn, cfn) __tg_f1(__tg_type(x) + __tg_type(y), fn, cfn)
#define __tg_f3(x, y, z, fn, cfn) \
  __tg_f1(__tg_type(x) + __tg_type(y) + __tg_type(z), fn, cfn)

// Macros that expand to both real and complex functions.
#define acos(x) __tg_f1(x, acos, cacos)(x)
#define asin(x) __tg_f1(x, asin, casin)(x)
#define atan(x) __tg_f1(x, atan, catan)(x)
#define acosh(x) __tg_f1(x, acosh, cacosh)(x)
#define asinh(x) __tg_f1(x, asinh, casinh)(x)
#define atanh(x) __tg_f1(x, atanh, catanh)(x)
#define cos(x) __tg_f1(x, cos, ccos)(x)
#define sin(x) __tg_f1(x, sin, csin)(x)
#define tan(x) __tg_f1(x, tan, ctan)(x)
#define cosh(x) __tg_f1(x, cosh, ccosh)(x)
#define sinh(x) __tg_f1(x, sinh, csinh)(x)
#define tanh(x) __tg_f1(x, tanh, ctanh)(x)
#define exp(x) __tg_f1(x, exp, cexp)(x)
#define log(x) __tg_f1(x, log, clog)(x)
#define pow(x, y) __tg_f2(x, y, pow, cpow)(x, y)
#define sqrt(x) __tg_f1(x, sqrt, csqrt)(x)
#define fabs(x) __tg_f1(x, fabs, cabs)(x)

// Macros that always expand to real functions.
#define atan2(x, y) __tg_f2(x, y, atan2, atan2)(x, y)
#define cbrt(x) __tg_f1(x, cbrt, cbrt)(x)
#define ceil(x) __tg_f1(x, ceil, ceil)(x)
#define copysign(x, y) __tg_f2(x, y, copysign, copysign)(x, y)
#define erf(x) __tg_f1(x, erf, erf)(x)
#define erfc(x) __tg_f1(x, erfc, erfc)(x)
#define exp2(x) __tg_f1(x, exp2, exp2)(x)
#define expm1(x) __tg_f1(x, expm1, expm1)(x)
#define fdim(x, y) __tg_f2(x, y, fdim, fdim)(x, y)
#define floor(x) __tg_f1(x, floor, floor)(x)
#define fma(x, y, z) __tg_f3(x, y, z, fma, fma)(x, y, z)
#define fmax(x, y) __tg_f2(x, y, fmax, fmax)(x, y)
#define fmin(x, y) __tg_f2(x, y, fmin, fmin)(x, y)
#define fmod(x, y) __tg_f2(x, y, fmod, fmod)(x, y)
#define frexp(x, y) __tg_f1(x, frexp, frexp)(x, y)
#define hypot(x, y) __tg_f2(x, y, hypot, hypot)(x, y)
#define ilogb(x) __tg_f1(x, ilogb, ilogb)(x)
#define ldexp(x, y) __tg_f1(x, ldexp, ldexp)(x, y)
#define lgamma(x) __tg_f1(x, lgamma, lgamma)(x)
#define llrint(x) __tg_f1(x, llrint, llrint)(x)
#define llround(x) __tg_f1(x, llround, llround)(x)
#define log10(x) __tg_f1(x, log10, log10)(x)
#define log1p(x) __tg_f1(x, log1p, log1p)(x)
#define log2(x) __tg_f1(x, log2, log2)(x)
#define logb(x) __tg_f1(x, logb, logb)(x)
#define lrint(x) __tg_f1(x, lrint, lrint)(x)
#define lround(x) __tg_f1(x, lround, lround)(x)
#define nearbyint(x) __tg_f1(x, nearbyint, nearbyint)(x)
#define nextafter(x, y) __tg_f2(x, y, nextafter, nextafter)(x, y)
#define nexttoward(x, y) __tg_f1(x, nexttoward, nexttoward)(x, y)
#define remainder(x, y) __tg_f2(x, y, remainder, remainder)(x, y)
#define remquo(x, y, z) __tg_f2(x, y, remquo, remquo)(x, y, z)
#define rint(x) __tg_f1(x, rint, rint)(x)
#define round(x) __tg_f1(x, round, round)(x)
#define scalbn(x, y) __tg_f1(x, scalbn, scalbn)(x, y)
#define scalbln(x, y) __tg_f1(x, scalbln, scalbln)(x, y)
#define tgamma(x) __tg_f1(x, tgamma, tgamma)(x)
#define trunc(x) __tg_f1(x, trunc, trunc)(x)

// Macros that always expand to complex functions.
#define carg(x) __tg_f1(x, carg, carg)(x)
#define cimag(x) __tg_f1(x, cimag, cimag)(x)
#define conj(x) __tg_f1(x, conj, conj)(x)
#define cproj(x) __tg_f1(x, cproj, cproj)(x)
#define creal(x) __tg_f1(x, creal, creal)(x)

#endif
