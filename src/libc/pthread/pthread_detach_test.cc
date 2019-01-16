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

static void *do_alive(void *arg) {
  EXPECT_EQ((void *)42, arg);
  do_sleep();
  return (void *)1337;
}

TEST(pthread_detach, alive) {
  pthread_t thread;
  ASSERT_EQ(0, pthread_create(&thread, NULL, do_alive, (void *)42));

  // The thread should still be alive when calling pthread_detach().
  ASSERT_EQ(0, pthread_detach(thread));
}

static void *do_dead(void *arg) {
  EXPECT_EQ((void *)9000, arg);
  return (void *)82588;
}

TEST(pthread_detach, dead) {
  pthread_t thread;
  ASSERT_EQ(0, pthread_create(&thread, NULL, do_dead, (void *)9000));

  do_sleep();

  // The thread should already be dead when calling pthread_detach().
  ASSERT_EQ(0, pthread_detach(thread));
}
