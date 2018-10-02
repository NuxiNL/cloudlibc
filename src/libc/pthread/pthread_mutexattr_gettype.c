// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>
#include <cloudlibc_interceptors.h>

int __cloudlibc_pthread_mutexattr_gettype(const pthread_mutexattr_t *restrict attr,
                              int *restrict type) {
  *type = attr->__type;
  return 0;
}

__weak_reference(__cloudlibc_pthread_mutexattr_gettype, pthread_mutexattr_gettype);
