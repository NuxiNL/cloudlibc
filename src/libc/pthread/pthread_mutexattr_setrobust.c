// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <pthread.h>

int pthread_mutexattr_setrobust(pthread_mutexattr_t *attr, int robust) {
  return robust == PTHREAD_MUTEX_STALLED ? 0 : EINVAL;
}
