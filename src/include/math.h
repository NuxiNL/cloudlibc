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

// <math.h> - mathematical declarations
//
// Extensions:
// - lgamma_r(), lgammaf_r() and lgammal_r():
//   Exposes signgam in a thread-safe way. Also present on most other
//   operating systems.

#ifndef _MATH_H_
#define _MATH_H_

#include <_/cdefs.h>
#include <_/limits.h>

#if defined(__FLT_EVAL_METHOD__) && __FLT_EVAL_METHOD__ == 0
typedef float float_t;
typedef double double_t;
#elif __FLT_EVAL_METHOD__ == 1
typedef double float_t;
typedef double double_t;
#elif __FLT_EVAL_METHOD__ == 2
typedef long double float_t;
typedef long double double_t;
#else
#error "Unknown __FLT_EVAL_METHOD__"
#endif

_Static_assert(sizeof(float_t) >= sizeof(float),
               "float_t has to be at least as wide as float");
_Static_assert(sizeof(double_t) >= sizeof(double),
               "double_t has to be at least as wide as double");
_Static_assert(sizeof(double_t) >= sizeof(float_t),
               "double_t has to be at least as wide as float_t");

#define fpclassify(x) \
  __builtin_fpclassify(FP_NAN, FP_INFINITE, FP_NORMAL, FP_SUBNORMAL, FP_ZERO, x)
#define isfinite(x) __builtin_isfinite(x)
#define isgreater(x, y) __builtin_isgreater(x, y)
#define isgreaterequal(x, y) __builtin_isgreaterequal(x, y)
#define isinf(x) __builtin_isinf(x)
#define isless(x, y) __builtin_isless(x, y)
#define islessequal(x, y) __builtin_islessequal(x, y)
#define islessgreater(x, y) __builtin_islessgreater(x, y)
#define isnan(x) __builtin_isnan(x)
#define isnormal(x) __builtin_isnormal(x)
#define isunordered(x, y) __builtin_isunordered(x, y)
// clang-format off
#define signbit(x)                             \
  _Generic(x,                                  \
           float: __builtin_signbitf,          \
           double: __builtin_signbit,          \
           long double: __builtin_signbitl)(x)
// clang-format on

#define M_E 2.7182818284590452353602874713526624977572  // e.
#define M_LOG2E 1.442695040888963407359924681001892137  // \log_{2} e.
#define M_LOG10E 0.43429448190325182765112891891660508  // \log_{10} e.
#define M_LN2 0.69314718055994530941723212145817656808  // \log_{e} 2.
#define M_LN10 2.3025850929940456840179914546843642076  // \log_{e} 10.
#define M_PI 3.141592653589793238462643383279502884197  // \pi.
#define M_PI_2 1.5707963267948966192313216916397514421  // \frac{\pi}{2}.
#define M_PI_4 0.7853981633974483096156608458198757210  // \frac{\pi}{4}.
#define M_1_PI 0.3183098861837906715377675267450287241  // \frac{1}{\pi}.
#define M_2_PI 0.6366197723675813430755350534900574481  // \frac{2}{\pi}.
#define M_2_SQRTPI 1.128379167095512573896158903121545  // \frac{2}{\sqrt{\pi}}.
#define M_SQRT2 1.414213562373095048801688724209698079  // \sqrt{2}.
#define M_SQRT1_2 0.7071067811865475244008443621048490  // \sqrt{\frac{1}{2}}.

#define HUGE_VAL __builtin_huge_val()
#define HUGE_VALF __builtin_huge_valf()
#define HUGE_VALL __builtin_huge_vall()
#define INFINITY __builtin_inff()
#define NAN __builtin_nanf("")

// Returned by fpclassify().
#define FP_INFINITE 1
#define FP_NAN 2
#define FP_NORMAL 3
#define FP_SUBNORMAL 4
#define FP_ZERO 5

// Returned by ilogb().
#define FP_ILOGB0 (-_INT_MAX)
#define FP_ILOGBNAN _INT_MAX

#define MATH_ERRNO 0x1
#define MATH_ERREXCEPT 0x2
#define math_errhandling MATH_ERREXCEPT

__BEGIN_DECLS
double acos(double);
float acosf(float);
double acosh(double);
float acoshf(float);
long double acoshl(long double);
long double acosl(long double);
double asin(double);
float asinf(float);
double asinh(double);
float asinhf(float);
long double asinhl(long double);
long double asinl(long double);
double atan(double);
double atan2(double, double);
float atan2f(float, float);
long double atan2l(long double, long double);
float atanf(float);
double atanh(double);
float atanhf(float);
long double atanhl(long double);
long double atanl(long double);
double cbrt(double);
float cbrtf(float);
long double cbrtl(long double);
double ceil(double);
float ceilf(float);
long double ceill(long double);
double copysign(double, double);
float copysignf(float, float);
long double copysignl(long double, long double);
double cos(double);
float cosf(float);
double cosh(double);
float coshf(float);
long double coshl(long double);
long double cosl(long double);
double erf(double);
double erfc(double);
float erfcf(float);
long double erfcl(long double);
float erff(float);
long double erfl(long double);
double exp(double);
double exp2(double);
float exp2f(float);
long double exp2l(long double);
float expf(float);
long double expl(long double);
double expm1(double);
float expm1f(float);
long double expm1l(long double);
double fabs(double);
float fabsf(float);
long double fabsl(long double);
double fdim(double, double);
float fdimf(float, float);
long double fdiml(long double, long double);
double floor(double);
float floorf(float);
long double floorl(long double);
double fma(double, double, double);
float fmaf(float, float, float);
long double fmal(long double, long double, long double);
double fmax(double, double);
float fmaxf(float, float);
long double fmaxl(long double, long double);
double fmin(double, double);
float fminf(float, float);
long double fminl(long double, long double);
double fmod(double, double);
float fmodf(float, float);
long double fmodl(long double, long double);
double frexp(double, int *);
float frexpf(float, int *);
long double frexpl(long double, int *);
double hypot(double, double);
float hypotf(float, float);
long double hypotl(long double, long double);
int ilogb(double);
int ilogbf(float);
int ilogbl(long double);
double j0(double);
double j1(double);
double jn(int, double);
double ldexp(double, int);
float ldexpf(float, int);
long double ldexpl(long double, int);
double lgamma(double);
float lgammaf(float);
long double lgammal(long double);
double lgamma_r(double, int *);
float lgammaf_r(float, int *);
long double lgammal_r(long double, int *);
long long llrint(double);
long long llrintf(float);
long long llrintl(long double);
long long llround(double);
long long llroundf(float);
long long llroundl(long double);
double log(double);
double log10(double);
float log10f(float);
long double log10l(long double);
double log1p(double);
float log1pf(float);
long double log1pl(long double);
double log2(double);
float log2f(float);
long double log2l(long double);
double logb(double);
float logbf(float);
long double logbl(long double);
float logf(float);
long double logl(long double);
long lrint(double);
long lrintf(float);
long lrintl(long double);
long lround(double);
long lroundf(float);
long lroundl(long double);
double modf(double, double *);
float modff(float, float *);
long double modfl(long double, long double *);
double nan(const char *);
float nanf(const char *);
long double nanl(const char *);
double nearbyint(double);
float nearbyintf(float);
long double nearbyintl(long double);
double nextafter(double, double);
float nextafterf(float, float);
long double nextafterl(long double, long double);
double nexttoward(double, long double);
float nexttowardf(float, long double);
long double nexttowardl(long double, long double);
double pow(double, double);
float powf(float, float);
long double powl(long double, long double);
double remainder(double, double);
float remainderf(float, float);
long double remainderl(long double, long double);
double remquo(double, double, int *);
float remquof(float, float, int *);
long double remquol(long double, long double, int *);
double rint(double);
float rintf(float);
long double rintl(long double);
double round(double);
float roundf(float);
long double roundl(long double);
double scalbln(double, long);
float scalblnf(float, long);
long double scalblnl(long double, long);
double scalbn(double, int);
float scalbnf(float, int);
long double scalbnl(long double, int);
double sin(double);
float sinf(float);
double sinh(double);
float sinhf(float);
long double sinhl(long double);
long double sinl(long double);
double sqrt(double);
float sqrtf(float);
long double sqrtl(long double);
double tan(double);
float tanf(float);
double tanh(double);
float tanhf(float);
long double tanhl(long double);
long double tanl(long double);
double tgamma(double);
float tgammaf(float);
long double tgammal(long double);
double trunc(double);
float truncf(float);
long double truncl(long double);
double y0(double);
double y1(double);
double yn(int, double);
__END_DECLS

#endif
