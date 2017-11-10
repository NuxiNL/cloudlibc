// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>

int pthread_barrierattr_init(pthread_barrierattr_t *attr) {
  // Set default values.
  attr->__pshared = PTHREAD_PROCESS_PRIVATE;
  return 0;
}
