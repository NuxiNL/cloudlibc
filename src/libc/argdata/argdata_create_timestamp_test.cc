// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <argdata.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>

#include "gtest/gtest.h"

TEST(argdata_create_timestamp, bad) {
  // Nanoseconds out of bounds.
  {
    struct timespec ts = {.tv_nsec = -1};
    ASSERT_EQ(NULL, argdata_create_timestamp(&ts));
    ASSERT_EQ(EINVAL, errno);
  }
  {
    struct timespec ts = {.tv_nsec = 1000000000};
    ASSERT_EQ(NULL, argdata_create_timestamp(&ts));
    ASSERT_EQ(EINVAL, errno);
  }
}

TEST(argdata_create_timestamp, random) {
  for (size_t i = 0; i < 1000; ++i) {
    // Create timestamp object.
    struct timespec ts1;
    arc4random_buf(&ts1.tv_sec, sizeof(ts1.tv_sec));
    ts1.tv_nsec = arc4random_uniform(1000000000);
    argdata_t *ad = argdata_create_timestamp(&ts1);
    ASSERT_NE(NULL, ad);

    // Validate that it holds the same timestamp value.
    struct timespec ts2;
    ASSERT_EQ(0, argdata_get_timestamp(ad, &ts2));
    ASSERT_EQ(ts1.tv_sec, ts2.tv_sec);
    ASSERT_EQ(ts1.tv_nsec, ts2.tv_nsec);
    argdata_free(ad);
  }
}
