// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <pthread.h>

int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate) {
  *detachstate = attr->__detachstate;
  return 0;
}
