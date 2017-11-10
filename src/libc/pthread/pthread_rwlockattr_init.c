// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>

int pthread_rwlockattr_init(pthread_rwlockattr_t *attr) {
  // Set default values.
  attr->__pshared = PTHREAD_PROCESS_PRIVATE;
  attr->__type = PTHREAD_MUTEX_DEFAULT;
  return 0;
}

__strong_reference(pthread_rwlockattr_init, pthread_mutexattr_init);
