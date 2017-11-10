// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <pthread.h>

int pthread_mutexattr_setrobust(pthread_mutexattr_t *attr, int robust) {
  return robust == PTHREAD_MUTEX_STALLED ? 0 : EINVAL;
}
