// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

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
