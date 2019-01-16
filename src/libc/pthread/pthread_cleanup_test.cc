// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>

#include "gtest/gtest.h"

static void set(void *v) {
  *(int *)v = 1;
}

TEST(pthread_cleanup, execute) {
  // Cleanup function should be executed by pthread_cleanup_pop().
  int v = 0;
  pthread_cleanup_push(set, &v);
  v = 2;
  pthread_cleanup_pop(1);
  ASSERT_EQ(1, v);
}

TEST(pthread_cleanup, no_execute) {
  // Cleanup function should not be executed.
  int v = 0;
  pthread_cleanup_push(set, &v);
  v = 2;
  pthread_cleanup_pop(0);
  ASSERT_EQ(2, v);
}

static void *setfunc(void *arg) {
  // Cleanup function should be executed by pthread_exit().
  pthread_cleanup_push(set, arg);
  pthread_exit(NULL);
  pthread_cleanup_pop(0);
}

TEST(pthread_cleanup, exit) {
  int v = 0;
  pthread_t thread;
  ASSERT_EQ(0, pthread_create(&thread, NULL, setfunc, &v));
  ASSERT_EQ(0, pthread_join(thread, NULL));
  ASSERT_EQ(1, v);
}
