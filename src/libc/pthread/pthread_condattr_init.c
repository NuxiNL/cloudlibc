// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <pthread.h>
#include <time.h>

int pthread_condattr_init(pthread_condattr_t *attr) {
  // Set default values.
  attr->__clock = CLOCK_REALTIME;
  attr->__pshared = PTHREAD_PROCESS_PRIVATE;
  return 0;
}
