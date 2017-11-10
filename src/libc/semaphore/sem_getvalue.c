// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <limits.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>

int sem_getvalue(sem_t *restrict sem, int *restrict sval) {
  // Extract value from semaphore.
  pthread_mutex_lock(&sem->__lock);
  uint32_t value = sem->__value;
  pthread_mutex_unlock(&sem->__lock);

  // Return value is a signed integer type. Return INT_MAX in case the
  // semaphore value won't fit.
  *sval = value > INT_MAX ? INT_MAX : value;
  return 0;
}
