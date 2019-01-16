// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <pthread.h>

#include "gtest/gtest.h"

TEST(pthread_mutexattr_setrobust, example) {
  pthread_mutexattr_t attr;
  ASSERT_EQ(0, pthread_mutexattr_init(&attr));

  // Unsupported value.
  ASSERT_EQ(EINVAL, pthread_mutexattr_setrobust(&attr, 0xdeadc0de));

  // Only supported value.
  ASSERT_EQ(0, pthread_mutexattr_setrobust(&attr, PTHREAD_MUTEX_STALLED));

  ASSERT_EQ(0, pthread_mutexattr_destroy(&attr));
}
