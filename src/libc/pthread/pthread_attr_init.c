// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/pthread.h>

#include <pthread.h>

int pthread_attr_init(pthread_attr_t *attr) {
  // Set default values.
  attr->__detachstate = PTHREAD_CREATE_JOINABLE;
  attr->__stacksize = PTHREAD_STACK_DEFAULT;
  return 0;
}
