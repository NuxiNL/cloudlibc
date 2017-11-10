// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <pthread.h>
#include <semaphore.h>

int sem_timedwait(sem_t *restrict sem,
                  const struct timespec *restrict abstime) {
  pthread_mutex_lock(&sem->__lock);
  while (sem->__value == 0) {
    // Wait until the semaphore becomes greater than zero.
    int error = pthread_cond_timedwait(&sem->__cond, &sem->__lock, abstime);
    if (error != 0) {
      pthread_mutex_unlock(&sem->__lock);
      errno = error;
      return -1;
    }
  }
  --sem->__value;
  pthread_mutex_unlock(&sem->__lock);
  return 0;
}
