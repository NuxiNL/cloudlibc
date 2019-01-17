// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <assert.h>
#include <complex.h>
#include <program.h>
#include <stdlib.h>

#define ASSERT_EQ(a, b) assert((a) == (b))

void program_main(const argdata_t *ad) {
  // Test whether all functions in <complex.h> are present.
  ASSERT_EQ(0.0, cabs(CMPLX(0.0, 0.0)));
  ASSERT_EQ(0.0f, cabsf(CMPLXF(0.0f, 0.0f)));
  ASSERT_EQ(0.0L, cabsl(CMPLXL(0.0L, 0.0L)));
#if 0  // TODO(ed): musl seems to give a wrong value for these?
  ASSERT_EQ(CMPLX(0.0, 0.0), cacos(CMPLX(1.0, 0.0)));
  ASSERT_EQ(CMPLXF(0.0f, 0.0f), cacosf(CMPLXF(1.0f, 0.0f)));
  ASSERT_EQ(CMPLX(0.0, 0.0), cacosh(CMPLX(1.0, 0.0)));
  ASSERT_EQ(CMPLXF(0.0f, 0.0f), cacoshf(CMPLXF(1.0f, 0.0f)));
  ASSERT_EQ(CMPLXF(0.0L, 0.0L), cacoshl(CMPLXL(1.0L, 0.0L)));
  ASSERT_EQ(CMPLXL(0.0L, 0.0L), cacosl(CMPLXL(1.0L, 0.0L)));
#endif
  ASSERT_EQ(0.0, carg(CMPLX(0.0, 0.0)));
  ASSERT_EQ(0.0f, cargf(CMPLXF(0.0f, 0.0f)));
  ASSERT_EQ(0.0L, cargl(CMPLXL(0.0L, 0.0L)));
  ASSERT_EQ(CMPLX(0.0, 0.0), casin(CMPLX(0.0, 0.0)));
  ASSERT_EQ(CMPLXF(0.0f, 0.0f), casinf(CMPLXF(0.0f, 0.0f)));
  ASSERT_EQ(CMPLX(0.0, 0.0), casinh(CMPLX(0.0, 0.0)));
  ASSERT_EQ(CMPLXF(0.0f, 0.0f), casinhf(CMPLXF(0.0f, 0.0f)));
  ASSERT_EQ(CMPLXL(0.0L, 0.0L), casinhl(CMPLXL(0.0L, 0.0L)));
  ASSERT_EQ(CMPLXL(0.0L, 0.0L), casinl(CMPLXL(0.0L, 0.0L)));
  ASSERT_EQ(CMPLX(0.0, 0.0), catan(CMPLX(0.0, 0.0)));
  ASSERT_EQ(CMPLXF(0.0f, 0.0f), catanf(CMPLXF(0.0f, 0.0f)));
  ASSERT_EQ(CMPLX(0.0, 0.0), catanh(CMPLX(0.0, 0.0)));
  ASSERT_EQ(CMPLXF(0.0f, 0.0f), catanhf(CMPLXF(0.0f, 0.0f)));
  ASSERT_EQ(CMPLXL(0.0L, 0.0L), catanhl(CMPLXL(0.0L, 0.0L)));
  ASSERT_EQ(CMPLXL(0.0L, 0.0L), catanl(CMPLXL(0.0L, 0.0L)));
  ASSERT_EQ(CMPLX(1.0, 0.0), ccos(CMPLX(0.0, 0.0)));
  ASSERT_EQ(CMPLXF(1.0f, 0.0f), ccosf(CMPLXF(0.0f, 0.0f)));
  ASSERT_EQ(CMPLX(1.0, 0.0), ccosh(CMPLX(0.0, 0.0)));
  ASSERT_EQ(CMPLXF(1.0f, 0.0f), ccoshf(CMPLXF(0.0f, 0.0f)));
  ASSERT_EQ(CMPLXL(1.0L, 0.0L), ccoshl(CMPLXL(0.0L, 0.0L)));
  ASSERT_EQ(CMPLXL(1.0L, 0.0L), ccosl(CMPLXL(0.0L, 0.0L)));
  ASSERT_EQ(CMPLX(1.0, 0.0), cexp(CMPLX(0.0, 0.0)));
  ASSERT_EQ(CMPLXF(1.0f, 0.0f), cexpf(CMPLXF(0.0f, 0.0f)));
  ASSERT_EQ(CMPLXL(1.0L, 0.0L), cexpl(CMPLXL(0.0L, 0.0L)));
  ASSERT_EQ(2.0, cimag(CMPLX(1.0, 2.0)));
  ASSERT_EQ(2.0f, cimagf(CMPLXF(1.0f, 2.0f)));
  ASSERT_EQ(2.0L, cimagl(CMPLXL(1.0L, 2.0L)));
  ASSERT_EQ(CMPLX(0.0, 0.0), clog(CMPLX(1.0, 0.0)));
  ASSERT_EQ(CMPLXF(0.0f, 0.0f), clogf(CMPLXF(1.0f, 0.0f)));
  ASSERT_EQ(CMPLXL(0.0L, 0.0L), clogl(CMPLXL(1.0L, 0.0L)));
  ASSERT_EQ(CMPLX(7.0, -5.0), conj(CMPLX(7.0, 5.0)));
  ASSERT_EQ(CMPLXF(7.0f, -5.0f), conjf(CMPLXF(7.0f, 5.0f)));
  ASSERT_EQ(CMPLXL(7.0L, -5.0L), conjl(CMPLXL(7.0L, 5.0L)));
  ASSERT_EQ(CMPLX(1.0, 0.0), cpow(CMPLX(1.0, 0.0), CMPLX(0.0, 0.0)));
  ASSERT_EQ(CMPLXF(1.0f, 0.0f), cpowf(CMPLXF(1.0f, 0.0f), CMPLXF(0.0f, 0.0f)));
  ASSERT_EQ(CMPLXL(1.0L, 0.0L), cpowl(CMPLXL(1.0L, 0.0L), CMPLXL(0.0L, 0.0L)));
  ASSERT_EQ(CMPLX(0.0, 0.0), cproj(CMPLX(0.0, 0.0)));
  ASSERT_EQ(CMPLXF(0.0f, 0.0f), cprojf(CMPLXF(0.0f, 0.0f)));
  ASSERT_EQ(CMPLXL(0.0L, 0.0L), cprojl(CMPLXL(0.0L, 0.0L)));
  ASSERT_EQ(1.0, creal(CMPLX(1.0, 2.0)));
  ASSERT_EQ(1.0f, crealf(CMPLXF(1.0f, 2.0f)));
  ASSERT_EQ(1.0L, creall(CMPLXL(1.0L, 2.0L)));
  ASSERT_EQ(CMPLX(0.0, 0.0), csin(CMPLX(0.0, 0.0)));
  ASSERT_EQ(CMPLXF(0.0f, 0.0f), csinf(CMPLXF(0.0f, 0.0f)));
  ASSERT_EQ(CMPLX(0.0, 0.0), csinh(CMPLX(0.0, 0.0)));
  ASSERT_EQ(CMPLXF(0.0f, 0.0f), csinhf(CMPLXF(0.0f, 0.0f)));
  ASSERT_EQ(CMPLXL(0.0L, 0.0L), csinhl(CMPLXL(0.0L, 0.0L)));
  ASSERT_EQ(CMPLXL(0.0L, 0.0L), csinl(CMPLXL(0.0L, 0.0L)));
  ASSERT_EQ(CMPLX(0.0, 0.0), csqrt(CMPLX(0.0, 0.0)));
  ASSERT_EQ(CMPLXF(0.0f, 0.0f), csqrtf(CMPLXF(0.0f, 0.0f)));
  ASSERT_EQ(CMPLXL(0.0L, 0.0L), csqrtl(CMPLXL(0.0L, 0.0L)));
  ASSERT_EQ(CMPLX(0.0, 0.0), ctan(CMPLX(0.0, 0.0)));
  ASSERT_EQ(CMPLXF(0.0f, 0.0f), ctanf(CMPLXF(0.0f, 0.0f)));
  ASSERT_EQ(CMPLX(0.0, 0.0), ctanh(CMPLX(0.0, 0.0)));
  ASSERT_EQ(CMPLXF(0.0f, 0.0f), ctanhf(CMPLXF(0.0f, 0.0f)));
  ASSERT_EQ(CMPLXL(0.0L, 0.0L), ctanhl(CMPLXL(0.0L, 0.0L)));
  ASSERT_EQ(CMPLXL(0.0L, 0.0L), ctanl(CMPLXL(0.0L, 0.0L)));

  exit(0);
}
