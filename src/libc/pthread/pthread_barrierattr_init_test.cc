// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>

#include "gtest/gtest.h"

TEST(pthread_barrierattr_init, defaults) {
  pthread_barrierattr_t attr;
  ASSERT_EQ(0, pthread_barrierattr_init(&attr));

  {
    int pshared;
    ASSERT_EQ(0, pthread_barrierattr_getpshared(&attr, &pshared));
    ASSERT_LE(PTHREAD_PROCESS_PRIVATE, pshared);
  }

  ASSERT_EQ(0, pthread_barrierattr_destroy(&attr));
}
