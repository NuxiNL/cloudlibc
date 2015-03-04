// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <pthread.h>
#include <semaphore.h>

int sem_destroy(sem_t *sem) {
  pthread_mutex_destroy(&sem->__lock);
  pthread_cond_destroy(&sem->__cond);
  return 0;
}
