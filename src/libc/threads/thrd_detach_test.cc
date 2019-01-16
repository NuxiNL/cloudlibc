// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <threads.h>
#include <time.h>

#include "gtest/gtest.h"

static void do_sleep(void) {
  struct timespec ts = {.tv_sec = 0, .tv_nsec = 100000000L};
  ASSERT_EQ(0, thrd_sleep(&ts));
}

static int do_alive(void *arg) {
  EXPECT_EQ((void *)42, arg);
  do_sleep();
  return 1337;
}

TEST(thrd_detach, alive) {
  thrd_t thread;
  ASSERT_EQ(thrd_success, thrd_create(&thread, do_alive, (void *)42));

  // The thread should still be alive when calling thrd_detach().
  ASSERT_EQ(thrd_success, thrd_detach(thread));
}

static int do_dead(void *arg) {
  EXPECT_EQ((void *)9000, arg);
  return 82588;
}

TEST(thrd_detach, dead) {
  thrd_t thread;
  ASSERT_EQ(thrd_success, thrd_create(&thread, do_dead, (void *)9000));

  do_sleep();

  // The thread should already be dead when calling thrd_detach().
  ASSERT_EQ(thrd_success, thrd_detach(thread));
}
