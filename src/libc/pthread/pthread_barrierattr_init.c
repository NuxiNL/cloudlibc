// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <pthread.h>

int pthread_barrierattr_init(pthread_barrierattr_t *attr) {
  // Set default values.
  attr->__pshared = PTHREAD_PROCESS_PRIVATE;
  return 0;
}
