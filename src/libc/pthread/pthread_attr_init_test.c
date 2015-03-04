// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <limits.h>
#include <pthread.h>
#include <testing.h>

TEST(pthread_attr_init, defaults) {
  pthread_attr_t attr;
  ASSERT_EQ(0, pthread_attr_init(&attr));

  {
    int detachstate;
    ASSERT_EQ(0, pthread_attr_getdetachstate(&attr, &detachstate));
    ASSERT_EQ(PTHREAD_CREATE_JOINABLE, detachstate);
  }

  {
    void *stackaddr;
    size_t stacksize;
    ASSERT_EQ(0, pthread_attr_getstack(&attr, &stackaddr, &stacksize));
    ASSERT_EQ(NULL, stackaddr);
    ASSERT_LE(PTHREAD_STACK_MIN, stacksize);
  }

  {
    size_t stacksize;
    ASSERT_EQ(0, pthread_attr_getstacksize(&attr, &stacksize));
    ASSERT_LE(PTHREAD_STACK_MIN, stacksize);
  }

  ASSERT_EQ(0, pthread_attr_destroy(&attr));
}
