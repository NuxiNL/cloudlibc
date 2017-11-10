// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <semaphore.h>

int sem_post(sem_t *sem) {
  pthread_mutex_lock(&sem->__lock);
  if (sem->__value == UINT_MAX) {
    // Incremeing the semaphore would cause an overflow.
    pthread_mutex_unlock(&sem->__lock);
    errno = EINVAL;
    return -1;
  }
  ++sem->__value;
  pthread_mutex_unlock(&sem->__lock);
  pthread_cond_signal(&sem->__cond);
  return 0;
}
