// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>
#include <cloudlibc_interceptors.h>

int __cloudlibc_pthread_attr_getstacksize(const pthread_attr_t *restrict attr,
                              size_t *restrict stacksize) {
  *stacksize = attr->__stacksize;
  return 0;
}

__weak_reference(__cloudlibc_pthread_attr_getstacksize, pthread_attr_getstacksize);
