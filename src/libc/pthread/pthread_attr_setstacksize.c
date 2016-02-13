// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <limits.h>
#include <pthread.h>

int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize) {
  if (stacksize < PTHREAD_STACK_MIN)
    return EINVAL;
  attr->__stacksize = stacksize;
  return 0;
}
