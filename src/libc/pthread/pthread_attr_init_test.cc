// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <limits.h>
#include <pthread.h>

#include "gtest/gtest.h"

TEST(pthread_attr_init, defaults) {
  pthread_attr_t attr;
  ASSERT_EQ(0, pthread_attr_init(&attr));

  {
    int detachstate;
    ASSERT_EQ(0, pthread_attr_getdetachstate(&attr, &detachstate));
    ASSERT_EQ(PTHREAD_CREATE_JOINABLE, detachstate);
  }

  {
    size_t stacksize;
    ASSERT_EQ(0, pthread_attr_getstacksize(&attr, &stacksize));
    ASSERT_LE(PTHREAD_STACK_MIN, stacksize);
  }

  ASSERT_EQ(0, pthread_attr_destroy(&attr));
}
