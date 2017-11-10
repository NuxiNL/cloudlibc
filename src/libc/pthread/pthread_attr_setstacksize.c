// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <limits.h>
#include <pthread.h>

int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize) {
  if (stacksize < PTHREAD_STACK_MIN)
    return EINVAL;
  attr->__stacksize = stacksize;
  return 0;
}
