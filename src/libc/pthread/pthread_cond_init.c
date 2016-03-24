// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <cloudabi_types.h>
#include <pthread.h>
#include <stdatomic.h>
#include <time.h>

int pthread_cond_init(pthread_cond_t *restrict cond,
                      const pthread_condattr_t *restrict attr) {
  atomic_init(&cond->__waiters, CLOUDABI_CONDVAR_HAS_NO_WAITERS);
  cond->__clock = attr != NULL ? attr->__clock : CLOCK_REALTIME;
  cond->__pshared = attr != NULL ? attr->__pshared : PTHREAD_PROCESS_PRIVATE;
  return 0;
}
