// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>
#include <cloudlibc_interceptors.h>

int __cloudlibc_pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate) {
  *detachstate = attr->__detachstate;
  return 0;
}

__weak_reference(__cloudlibc_pthread_attr_getdetachstate, pthread_attr_getdetachstate);
