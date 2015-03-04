// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/float.h>

void __float2_normalize(struct float2 *f2) {
  if (f2->significand[0] == 0) {
    if (f2->significand[1] == 0)
      return;

    // Shortcut: shift 64 bits in one go.
    f2->significand[0] = f2->significand[1];
    f2->significand[1] = 0;
    f2->exponent -= 64;
  }

  // Iteratively normalize the number until the top bit is set.
  while ((f2->significand[0] & 0x8000000000000000) == 0) {
    f2->significand[0] = (f2->significand[0] << 1) | (f2->significand[1] >> 63);
    f2->significand[1] <<= 1;
    --f2->exponent;
  }
}
