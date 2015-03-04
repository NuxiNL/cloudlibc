// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <pthread.h>

int pthread_barrier_init(pthread_barrier_t *restrict barrier,
                         const pthread_barrierattr_t *restrict attr,
                         unsigned int count) {
  // Barrier with zero threads is meaningless.
  if (count == 0)
    return EINVAL;

  pthread_mutex_init(&barrier->__lock, NULL);
  pthread_cond_init(&barrier->__cond, NULL);
  barrier->__init = barrier->__remaining = count - 1;
  barrier->__generation = 0;
  return 0;
}
