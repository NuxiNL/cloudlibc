// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <limits.h>
#include <pthread.h>

#include "gtest/gtest.h"

TEST(pthread_attr_setstacksize, einval) {
  // Stack smaller than PTHREAD_STACK_MIN.
  pthread_attr_t attr;
  ASSERT_EQ(0, pthread_attr_init(&attr));
  ASSERT_EQ(EINVAL, pthread_attr_setstacksize(&attr, 0));
  ASSERT_EQ(EINVAL, pthread_attr_setstacksize(&attr, PTHREAD_STACK_MIN - 1));
  ASSERT_EQ(0, pthread_attr_init(&attr));
}

static void *do_nothing(void *arg) {
  return NULL;
}

TEST(pthread_attr_setstacksize, example) {
  // Spawn a thread with a custom stack size.
  pthread_attr_t attr;
  ASSERT_EQ(0, pthread_attr_init(&attr));
  ASSERT_EQ(0, pthread_attr_setstacksize(&attr, PTHREAD_STACK_MIN));

  pthread_t thread;
  ASSERT_EQ(0, pthread_create(&thread, &attr, do_nothing, NULL));
  ASSERT_EQ(0, pthread_attr_destroy(&attr));
  ASSERT_EQ(0, pthread_join(thread, NULL));
}
