// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <pthread.h>

#include "gtest/gtest.h"

TEST(pthread_mutexattr_settype, bad) {
  pthread_mutexattr_t attr;
  ASSERT_EQ(0, pthread_mutexattr_init(&attr));
  ASSERT_EQ(EINVAL, pthread_mutexattr_settype(&attr, 0xdeadc0de));
  ASSERT_EQ(0, pthread_mutexattr_destroy(&attr));
}

TEST(pthread_mutexattr_settype, normal) {
  // Create a normal mutex.
  pthread_mutexattr_t attr;
  ASSERT_EQ(0, pthread_mutexattr_init(&attr));
  ASSERT_EQ(0, pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_NORMAL));
  pthread_mutex_t mutex;
  ASSERT_EQ(0, pthread_mutex_init(&mutex, &attr));
  ASSERT_EQ(0, pthread_mutexattr_destroy(&attr));

  // We should be able to lock it once.
  ASSERT_EQ(0, pthread_mutex_lock(&mutex));

  // We should also unlock it once.
  ASSERT_EQ(0, pthread_mutex_unlock(&mutex));

  // As it is unlocked, destroying should work.
  ASSERT_EQ(0, pthread_mutex_destroy(&mutex));
}

TEST(pthread_mutexattr_settype, recursive) __no_lock_analysis {
  // Create a recursive mutex.
  pthread_mutexattr_t attr;
  ASSERT_EQ(0, pthread_mutexattr_init(&attr));
  ASSERT_EQ(0, pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE));
  pthread_mutex_t mutex;
  ASSERT_EQ(0, pthread_mutex_init(&mutex, &attr));
  ASSERT_EQ(0, pthread_mutexattr_destroy(&attr));

  // We should be able to lock it recursively.
  ASSERT_EQ(0, pthread_mutex_lock(&mutex));
  ASSERT_EQ(0, pthread_mutex_lock(&mutex));
  ASSERT_EQ(0, pthread_mutex_lock(&mutex));

  // We should also unlock it the same number of times.
  ASSERT_EQ(0, pthread_mutex_unlock(&mutex));
  ASSERT_EQ(0, pthread_mutex_unlock(&mutex));
  ASSERT_EQ(0, pthread_mutex_unlock(&mutex));

  // As it is unlocked, destroying should work.
  ASSERT_EQ(0, pthread_mutex_destroy(&mutex));
}
