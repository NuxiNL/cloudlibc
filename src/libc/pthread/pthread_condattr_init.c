// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>
#include <time.h>

int pthread_condattr_init(pthread_condattr_t *attr) {
  // Set default values.
  attr->__clock = CLOCK_REALTIME;
  attr->__pshared = PTHREAD_PROCESS_PRIVATE;
  return 0;
}
