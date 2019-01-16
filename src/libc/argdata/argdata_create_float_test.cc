// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <argdata.h>
#include <stdlib.h>
#include <string.h>

#include "gtest/gtest.h"

TEST(argdata_create_float, example) {
  for (size_t i = 0; i < 1000; ++i) {
    // Create new floating point object.
    double j;
    arc4random_buf(&j, sizeof(j));
    argdata_t *ad = argdata_create_float(j);

    // Read the value back in.
    double k;
    ASSERT_EQ(0, argdata_get_float(ad, &k));
    ASSERT_TRUE(memcmp(&j, &k, sizeof(double)) == 0);
    argdata_free(ad);
  }
}
