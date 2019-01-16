// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <argdata.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>

#include "gtest/gtest.h"

TEST(argdata_create_int, unsigned) {
  for (size_t i = 0; i < 1000; ++i) {
    // Create new integer object.
    uintmax_t j;
    arc4random_buf(&j, sizeof(j));
    argdata_t *ad = argdata_create_int(j);

    // Read the value back in as an unsigned integer.
    uintmax_t k;
    ASSERT_EQ(0, argdata_get_int(ad, &k));
    ASSERT_EQ(j, k);

    if (j <= INTMAX_MAX) {
      // Value can be read in as a signed integer.
      intmax_t l;
      ASSERT_EQ(0, argdata_get_int(ad, &l));
      ASSERT_EQ(j, l);
    } else {
      // Value cannot be read in as a signed integer.
      ASSERT_EQ(ERANGE, argdata_get_int(ad, (intmax_t *)NULL));
    }
    argdata_free(ad);
  }
}

TEST(argdata_create_int, signed) {
  for (size_t i = 0; i < 1000; ++i) {
    // Create new integer object.
    intmax_t j;
    arc4random_buf(&j, sizeof(j));
    argdata_t *ad = argdata_create_int(j);

    // Read the value back in as a signed integer.
    intmax_t k;
    ASSERT_EQ(0, argdata_get_int(ad, &k));
    ASSERT_EQ(j, k);

    if (j >= 0) {
      // Value can be read in as an unsigned integer.
      uintmax_t l;
      ASSERT_EQ(0, argdata_get_int(ad, &l));
      ASSERT_EQ(j, l);
    } else {
      // Value cannot be read in as an unsigned integer.
      ASSERT_EQ(ERANGE, argdata_get_int(ad, (uintmax_t *)NULL));
    }
    argdata_free(ad);
  }
}
