// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>

#include "gtest/gtest.h"

TEST(pthread_rwlockattr_init, defaults) {
  pthread_rwlockattr_t attr;
  ASSERT_EQ(0, pthread_rwlockattr_init(&attr));

  {
    int pshared;
    ASSERT_EQ(0, pthread_rwlockattr_getpshared(&attr, &pshared));
    ASSERT_LE(PTHREAD_PROCESS_PRIVATE, pshared);
  }

  ASSERT_EQ(0, pthread_rwlockattr_destroy(&attr));
}
