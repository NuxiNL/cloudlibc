// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <pthread.h>
#include <testing.h>

static void *do_nothing(void *arg) {
  return NULL;
}

TEST(pthread_attr_setdetachstate, example) {
  pthread_attr_t attr;
  ASSERT_EQ(0, pthread_attr_init(&attr));
  ASSERT_EQ(0, pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED));

  int detachstate;
  ASSERT_EQ(0, pthread_attr_getdetachstate(&attr, &detachstate));
  ASSERT_EQ(PTHREAD_CREATE_DETACHED, detachstate);

  for (int i = 0; i < 10; ++i) {
    pthread_t thread;
    ASSERT_EQ(0, pthread_create(&thread, &attr, do_nothing, NULL));
  }

  ASSERT_EQ(0, pthread_attr_destroy(&attr));
}
