// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/pthread.h>

#include <pthread.h>

int pthread_attr_init(pthread_attr_t *attr) {
  // Set default values.
  attr->__detachstate = PTHREAD_CREATE_JOINABLE;
  attr->__stacksize = PTHREAD_STACK_DEFAULT;
  return 0;
}
