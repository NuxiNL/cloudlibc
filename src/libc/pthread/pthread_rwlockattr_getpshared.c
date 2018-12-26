// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>
#include <cloudlibc_interceptors.h>

int __cloudlibc_pthread_rwlockattr_getpshared(const pthread_rwlockattr_t *restrict attr,
                                  int *restrict pshared) {
  *pshared = attr->__pshared;
  return 0;
}

__strong_reference(__cloudlibc_pthread_rwlockattr_getpshared, __cloudlibc_pthread_mutexattr_getpshared);
__weak_reference(__cloudlibc_pthread_rwlockattr_getpshared, pthread_rwlockattr_getpshared);
__weak_reference(__cloudlibc_pthread_mutexattr_getpshared, pthread_mutexattr_getpshared);
