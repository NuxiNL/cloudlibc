// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <pthread.h>
#include <semaphore.h>

int sem_trywait(sem_t *sem) {
  pthread_mutex_lock(&sem->__lock);
  if (sem->__value == 0) {
    pthread_mutex_unlock(&sem->__lock);
    errno = EAGAIN;
    return -1;
  }
  --sem->__value;
  pthread_mutex_unlock(&sem->__lock);
  return 0;
}
