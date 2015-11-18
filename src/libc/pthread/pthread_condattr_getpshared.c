// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <pthread.h>

int pthread_condattr_getpshared(const pthread_condattr_t *restrict attr,
                                int *restrict pshared) {
  *pshared = attr->__pshared;
  return 0;
}
