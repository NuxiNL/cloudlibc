// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>
#include <semaphore.h>

int sem_wait(sem_t *sem) {
  pthread_mutex_lock(&sem->__lock);
  while (sem->__value == 0) {
    // Wait until the semaphore becomes greater than zero.
    pthread_cond_wait(&sem->__cond, &sem->__lock);
  }
  --sem->__value;
  pthread_mutex_unlock(&sem->__lock);
  return 0;
}
