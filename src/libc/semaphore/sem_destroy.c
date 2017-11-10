// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>
#include <semaphore.h>

int sem_destroy(sem_t *sem) {
  pthread_mutex_destroy(&sem->__lock);
  pthread_cond_destroy(&sem->__cond);
  return 0;
}
