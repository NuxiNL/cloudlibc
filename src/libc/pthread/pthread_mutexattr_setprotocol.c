// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <pthread.h>

int pthread_mutexattr_setprotocol(pthread_mutexattr_t *attr, int protocol) {
  return protocol == PTHREAD_PRIO_INHERIT ? 0 : EINVAL;
}
