// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <pthread.h>
#include <testing.h>

TEST(pthread_mutexattr_init, defaults) {
  pthread_mutexattr_t attr;
  ASSERT_EQ(0, pthread_mutexattr_init(&attr));

  // Non-standard: should be PTHREAD_PRIO_NONE.
  {
    int protocol;
    ASSERT_EQ(0, pthread_mutexattr_getprotocol(&attr, &protocol));
    ASSERT_LE(PTHREAD_PRIO_INHERIT, protocol);
  }

  {
    int pshared;
    ASSERT_EQ(0, pthread_mutexattr_getpshared(&attr, &pshared));
    ASSERT_LE(PTHREAD_PROCESS_PRIVATE, pshared);
  }

  {
    int robust;
    ASSERT_EQ(0, pthread_mutexattr_getrobust(&attr, &robust));
    ASSERT_LE(PTHREAD_MUTEX_STALLED, robust);
  }

  {
    int type;
    ASSERT_EQ(0, pthread_mutexattr_gettype(&attr, &type));
    ASSERT_LE(PTHREAD_MUTEX_DEFAULT, type);
  }

  ASSERT_EQ(0, pthread_mutexattr_destroy(&attr));
}
