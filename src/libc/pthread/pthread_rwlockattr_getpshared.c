// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <pthread.h>

int pthread_rwlockattr_getpshared(const pthread_rwlockattr_t *restrict attr,
                                  int *restrict pshared) {
  *pshared = attr->__pshared;
  return 0;
}

__strong_reference(pthread_rwlockattr_getpshared, pthread_mutexattr_getpshared);
