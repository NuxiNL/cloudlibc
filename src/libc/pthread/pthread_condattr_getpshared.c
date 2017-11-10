// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>

int pthread_condattr_getpshared(const pthread_condattr_t *restrict attr,
                                int *restrict pshared) {
  *pshared = attr->__pshared;
  return 0;
}
