// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>

int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate) {
  *detachstate = attr->__detachstate;
  return 0;
}
