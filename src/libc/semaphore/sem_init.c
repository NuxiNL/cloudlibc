// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <pthread.h>
#include <semaphore.h>

int sem_init(sem_t *sem, int pshared, unsigned int value) {
  pthread_mutex_init(&sem->__lock, NULL);
  pthread_cond_init(&sem->__cond, NULL);
  sem->__value = value;
  return 0;
}
