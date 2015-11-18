// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <pthread.h>
#include <testing.h>

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
