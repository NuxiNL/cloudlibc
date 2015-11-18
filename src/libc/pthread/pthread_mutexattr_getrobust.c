// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <pthread.h>

int pthread_mutexattr_getrobust(const pthread_mutexattr_t *restrict attr,
                                int *restrict robust) {
  *robust = PTHREAD_MUTEX_STALLED;
  return 0;
}
