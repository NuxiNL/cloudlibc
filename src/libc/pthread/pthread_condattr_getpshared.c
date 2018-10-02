// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>
#include <cloudlibc_interceptors.h>

int __cloudlibc_pthread_condattr_getpshared(const pthread_condattr_t *restrict attr,
                                int *restrict pshared) {
  *pshared = attr->__pshared;
  return 0;
}

__weak_reference(__cloudlibc_pthread_condattr_getpshared, pthread_condattr_getpshared);
