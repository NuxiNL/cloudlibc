// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <pthread.h>
#include <testing.h>
#include <time.h>

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
