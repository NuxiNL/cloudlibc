// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/clock.h>

#include <errno.h>
#include <pthread.h>

int pthread_condattr_setclock(pthread_condattr_t *attr, clockid_t clock_id) {
  attr->__clock = clock_id;
  return 0;
}
