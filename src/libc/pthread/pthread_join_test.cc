// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>
#include <time.h>

#include "gtest/gtest.h"

static void do_sleep(void) {
  struct timespec ts = {.tv_sec = 0, .tv_nsec = 100000000L};
  ASSERT_EQ(0, clock_nanosleep(CLOCK_MONOTONIC, 0, &ts));
}

static void *do_block(void *arg) {
  EXPECT_EQ((void *)42, arg);
  do_sleep();
  return (void *)1337;
}

TEST(pthread_join, block) {
  pthread_t thread;
  ASSERT_EQ(0, pthread_create(&thread, NULL, do_block, (void *)42));

  // This should briefly block until the thread is terminated.
  void *value;
  ASSERT_EQ(0, pthread_join(thread, &value));
  ASSERT_EQ((void *)1337, value);
}

static void *do_noblock(void *arg) {
  EXPECT_EQ((void *)9000, arg);
  return (void *)82588;
}

TEST(pthread_join, noblock) {
  pthread_t thread;
  ASSERT_EQ(0, pthread_create(&thread, NULL, do_noblock, (void *)9000));

  do_sleep();

  // The thread should already be terminated.
  void *value;
  ASSERT_EQ(0, pthread_join(thread, &value));
  ASSERT_EQ((void *)82588, value);
}
