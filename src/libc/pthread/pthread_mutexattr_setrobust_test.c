// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <pthread.h>
#include <testing.h>

TEST(pthread_mutexattr_setrobust, example) {
  pthread_mutexattr_t attr;
  ASSERT_EQ(0, pthread_mutexattr_init(&attr));

  // Unsupported value.
  ASSERT_EQ(EINVAL, pthread_mutexattr_setrobust(&attr, 0xdeadc0de));

  // Only supported value.
  ASSERT_EQ(0, pthread_mutexattr_setrobust(&attr, PTHREAD_MUTEX_STALLED));

  ASSERT_EQ(0, pthread_mutexattr_destroy(&attr));
}
