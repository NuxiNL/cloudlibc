// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <pthread.h>

int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate) {
  switch (detachstate) {
    case PTHREAD_CREATE_DETACHED:
    case PTHREAD_CREATE_JOINABLE:
      attr->__detachstate = detachstate;
      return 0;
    default:
      return EINVAL;
  }
}
