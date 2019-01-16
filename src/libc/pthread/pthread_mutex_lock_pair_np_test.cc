// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>

#include "gtest/gtest.h"

TEST(pthread_mutex_lock_pair_np, example) __no_lock_analysis {
  pthread_mutex_t mutex1;
  ASSERT_EQ(0, pthread_mutex_init(&mutex1, NULL));
  pthread_mutex_t mutex2;
  ASSERT_EQ(0, pthread_mutex_init(&mutex2, NULL));

  // Both locks should be acquired.
  ASSERT_EQ(0, pthread_mutex_lock_pair_np(&mutex1, &mutex2));
  ASSERT_EQ(0, pthread_mutex_unlock(&mutex1));
  ASSERT_EQ(0, pthread_mutex_unlock(&mutex2));

  ASSERT_EQ(0, pthread_mutex_destroy(&mutex1));
  ASSERT_EQ(0, pthread_mutex_destroy(&mutex2));
}
