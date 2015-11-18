// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <pthread.h>
#include <threads.h>

int pthread_equal(pthread_t t1, pthread_t t2) {
  return t1 == t2;
}

__strong_reference(pthread_equal, thrd_equal);
