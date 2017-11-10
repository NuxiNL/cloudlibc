// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>

int pthread_rwlockattr_destroy(pthread_rwlockattr_t *attr) {
  return 0;
}

__strong_reference(pthread_rwlockattr_destroy, pthread_mutexattr_destroy);
