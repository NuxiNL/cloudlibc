// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/clock.h>

#include <errno.h>
#include <pthread.h>

int pthread_condattr_setclock(pthread_condattr_t *attr, clockid_t clock_id) {
  attr->__clock = clock_id;
  return 0;
}
