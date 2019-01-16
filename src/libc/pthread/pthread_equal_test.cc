// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>

#include "gtest/gtest.h"

TEST(pthread_equal, equal) {
  ASSERT_TRUE(pthread_equal(pthread_self(), pthread_self()));
}

static void *do_nothing(void *arg) {
  return NULL;
}

TEST(pthread_equal, unequal) {
  pthread_t thread;
  ASSERT_EQ(0, pthread_create(&thread, NULL, do_nothing, NULL));
  ASSERT_FALSE(pthread_equal(pthread_self(), thread));
  ASSERT_EQ(0, pthread_join(thread, NULL));
}
