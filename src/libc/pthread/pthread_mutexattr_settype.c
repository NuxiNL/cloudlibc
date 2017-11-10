// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <pthread.h>

int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int type) {
  switch (type) {
    case PTHREAD_MUTEX_NORMAL:
    case PTHREAD_MUTEX_RECURSIVE:
      attr->__type = type;
      return 0;
    default:
      return EINVAL;
  }
}
