// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/float.h>

int __float10_to_float2(const struct float10 *f10, struct float2 *f2) {
  // Zero value float. Terminate immediately.
  if (f10->significand[0] == 0 && f10->significand[1] == 0) {
    *f2 = (struct float2){};
    return 0;
  }

  // Start out by initializing our floating point number with the
  // significand from the base-10 floating point number.
  *f2 = (struct float2){
      .significand = {f10->significand[0], f10->significand[1]}, .exponent = 0};

  // Keep on multiplying/dividing by a factor 10 based on the exponent
  // of the base-10 floating point number.
  int16_t exp10 = f10->exponent;
  while (exp10 > 0) {
    --exp10;
  }
  return 0;
}
