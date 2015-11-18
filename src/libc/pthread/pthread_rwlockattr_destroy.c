// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <pthread.h>

int pthread_rwlockattr_destroy(pthread_rwlockattr_t *attr) {
  return 0;
}

__strong_reference(pthread_rwlockattr_destroy, pthread_mutexattr_destroy);
