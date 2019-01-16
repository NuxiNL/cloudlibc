// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>
#include <time.h>

#include "gtest/gtest.h"

TEST(pthread_condattr_init, defaults) {
  pthread_condattr_t attr;
  ASSERT_EQ(0, pthread_condattr_init(&attr));

  {
    clockid_t clock_id;
    ASSERT_EQ(0, pthread_condattr_getclock(&attr, &clock_id));
    ASSERT_LE(CLOCK_REALTIME, clock_id);
  }

  {
    int pshared;
    ASSERT_EQ(0, pthread_condattr_getpshared(&attr, &pshared));
    ASSERT_LE(PTHREAD_PROCESS_PRIVATE, pshared);
  }

  ASSERT_EQ(0, pthread_condattr_destroy(&attr));
}
