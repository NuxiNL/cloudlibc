// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <argdata.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>

#include "gtest/gtest.h"

TEST(argdata_create_fd, bad) {
  // Bad file descriptors.
  ASSERT_EQ(NULL, argdata_create_fd(-1));
  ASSERT_EQ(EBADF, errno);
  ASSERT_EQ(NULL, argdata_create_fd(INT_MIN));
  ASSERT_EQ(EBADF, errno);
}

TEST(argdata_create_fd, good) {
  for (size_t i = 0; i < 1000; ++i) {
    // Create new file descriptor object.
    int j = arc4random_uniform(INT_MAX);
    argdata_t *ad = argdata_create_fd(j);

    // Read the value back in.
    int k;
    ASSERT_EQ(0, argdata_get_fd(ad, &k));
    ASSERT_EQ(j, k);
    argdata_free(ad);
  }
}
