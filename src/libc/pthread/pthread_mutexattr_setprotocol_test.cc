// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <pthread.h>

#include "gtest/gtest.h"

TEST(pthread_mutexattr_setprotocol, example) {
  pthread_mutexattr_t attr;
  ASSERT_EQ(0, pthread_mutexattr_init(&attr));

  // Unsupported value.
  ASSERT_EQ(EINVAL, pthread_mutexattr_setprotocol(&attr, 0xdeadc0de));

  // Only supported value.
  ASSERT_EQ(0, pthread_mutexattr_setprotocol(&attr, PTHREAD_PRIO_INHERIT));

  ASSERT_EQ(0, pthread_mutexattr_destroy(&attr));
}
