// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>
#include <threads.h>

int pthread_equal(pthread_t t1, pthread_t t2) {
  return t1 == t2;
}

__strong_reference(pthread_equal, thrd_equal);
