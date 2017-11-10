// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <assert.h>
#include <stddef.h>
#include <tgmath.h>

// Test <tgmath.h> macros based on the return type of the function.
//
// Most of the functions provided by <tgmath.h> have a different return
// type depending on the arguments. We can use this to easily test
// whether an invocation of a macro yields a call to the correct
// instance of the <math.h>/<complex.h> function.
//
// This approach does not allow us to test ilogb(), llrint(), llround(),
// lrint() and lround().
#define EXPECT_RETURN_TYPE(e, t) \
  static_assert(_Generic(e, t : 1), "Incorrect return type")

// Function always returns a real floating point type.
#define REAL_1(f, ...)                                             \
  EXPECT_RETURN_TYPE(f(1.0f, ##__VA_ARGS__), float);               \
  EXPECT_RETURN_TYPE(f(1.0, ##__VA_ARGS__), double);               \
  EXPECT_RETURN_TYPE(f(1, ##__VA_ARGS__), double);                 \
  EXPECT_RETURN_TYPE(f(1.0l, ##__VA_ARGS__), long double);         \
  EXPECT_RETURN_TYPE(f(CMPLXF(1.0f, 1.0f), ##__VA_ARGS__), float); \
  EXPECT_RETURN_TYPE(f(CMPLX(1.0, 1.0), ##__VA_ARGS__), double);   \
  EXPECT_RETURN_TYPE(f(CMPLXL(1.0l, 1.0l), ##__VA_ARGS__), long double)

#define REAL_2(f, ...)                                                   \
  EXPECT_RETURN_TYPE(f(1.0f, 1.0f, ##__VA_ARGS__), float);               \
  EXPECT_RETURN_TYPE(f(1.0, 1.0f, ##__VA_ARGS__), double);               \
  EXPECT_RETURN_TYPE(f(1.0f, 1, ##__VA_ARGS__), double);                 \
  EXPECT_RETURN_TYPE(f(1.0f, 1.0l, ##__VA_ARGS__), long double);         \
  EXPECT_RETURN_TYPE(f(CMPLXF(1.0f, 1.0f), 1.0f, ##__VA_ARGS__), float); \
  EXPECT_RETURN_TYPE(f(CMPLX(1.0, 1.0), 1, ##__VA_ARGS__), double);      \
  EXPECT_RETURN_TYPE(f(CMPLXL(1.0l, 1.0l), 1, ##__VA_ARGS__), long double)

#define REAL_3(f)                                               \
  EXPECT_RETURN_TYPE(f(1.0f, 1.0f, 1.0f), float);               \
  EXPECT_RETURN_TYPE(f(1.0f, 1.0f, 1.0), double);               \
  EXPECT_RETURN_TYPE(f(1, 1.0f, 1.0f), double);                 \
  EXPECT_RETURN_TYPE(f(1.0, 1.0l, 1.0f), long double);          \
  EXPECT_RETURN_TYPE(f(CMPLXF(1.0f, 1.0f), 1.0f, 1.0f), float); \
  EXPECT_RETURN_TYPE(f(1.0f, CMPLX(1.0, 1.0), 1.0f), double);   \
  EXPECT_RETURN_TYPE(f(1.0, 1, CMPLXL(1.0l, 1.0l)), long double)

// Function always returns a complex floating point type.
#define COMPLEX_1(f)                                        \
  EXPECT_RETURN_TYPE(f(1.0f), float complex);               \
  EXPECT_RETURN_TYPE(f(1.0), double complex);               \
  EXPECT_RETURN_TYPE(f(1), double complex);                 \
  EXPECT_RETURN_TYPE(f(1.0l), long double complex);         \
  EXPECT_RETURN_TYPE(f(CMPLXF(1.0f, 1.0f)), float complex); \
  EXPECT_RETURN_TYPE(f(CMPLX(1.0, 1.0)), double complex);   \
  EXPECT_RETURN_TYPE(f(CMPLXL(1.0l, 1.0l)), long double complex)

// Function returns a real or complex floating point type.
#define REAL_COMPLEX_1(f)                                   \
  EXPECT_RETURN_TYPE(f(1.0f), float);                       \
  EXPECT_RETURN_TYPE(f(1.0), double);                       \
  EXPECT_RETURN_TYPE(f(1), double);                         \
  EXPECT_RETURN_TYPE(f(1.0l), long double);                 \
  EXPECT_RETURN_TYPE(f(CMPLXF(1.0f, 1.0f)), float complex); \
  EXPECT_RETURN_TYPE(f(CMPLX(1.0, 1.0)), double complex);   \
  EXPECT_RETURN_TYPE(f(CMPLXL(1.0l, 1.0l)), long double complex)

#define REAL_COMPLEX_2(f)                                         \
  EXPECT_RETURN_TYPE(f(1.0f, 1.0f), float);                       \
  EXPECT_RETURN_TYPE(f(1.0, 1.0f), double);                       \
  EXPECT_RETURN_TYPE(f(1.0f, 1), double);                         \
  EXPECT_RETURN_TYPE(f(1.0l, 1.0), long double);                  \
  EXPECT_RETURN_TYPE(f(1.0f, CMPLXF(1.0f, 1.0f)), float complex); \
  EXPECT_RETURN_TYPE(f(1, CMPLX(1.0f, 1.0f)), double complex);    \
  EXPECT_RETURN_TYPE(f(CMPLXL(1.0l, 1.0l), 1.0), long double complex)

REAL_COMPLEX_1(acos);
REAL_COMPLEX_1(asin);
REAL_COMPLEX_1(atan);
REAL_COMPLEX_1(acosh);
REAL_COMPLEX_1(asinh);
REAL_COMPLEX_1(atanh);
REAL_COMPLEX_1(cos);
REAL_COMPLEX_1(sin);
REAL_COMPLEX_1(tan);
REAL_COMPLEX_1(cosh);
REAL_COMPLEX_1(sinh);
REAL_COMPLEX_1(tanh);
REAL_COMPLEX_1(exp);
REAL_COMPLEX_1(log);
REAL_COMPLEX_2(pow);
REAL_COMPLEX_1(sqrt);
REAL_1(fabs);

REAL_2(atan2);
REAL_1(cbrt);
REAL_1(ceil);
REAL_2(copysign);
REAL_1(erf);
REAL_1(erfc);
REAL_1(exp2);
REAL_1(expm1);
REAL_2(fdim);
REAL_1(floor);
REAL_3(fma);
REAL_2(fmax);
REAL_2(fmin);
REAL_2(fmod);
REAL_1(frexp, NULL);
REAL_2(hypot);
REAL_1(ldexp, 0);
REAL_1(lgamma);
REAL_1(log10);
REAL_1(log1p);
REAL_1(log2);
REAL_1(logb);
REAL_1(nearbyint);
REAL_2(nextafter);
REAL_1(nexttoward, 1.0L);
REAL_2(remainder);
REAL_2(remquo, NULL);
REAL_1(rint);
REAL_1(round);
REAL_1(scalbn, 0);
REAL_1(scalbln, 0);
REAL_1(tgamma);
REAL_1(trunc);

REAL_1(carg);
REAL_1(cimag);
COMPLEX_1(conj);
COMPLEX_1(cproj);
REAL_1(creal);
