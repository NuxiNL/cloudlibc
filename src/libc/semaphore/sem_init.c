// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <cloudabi_types.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdatomic.h>

int sem_init(sem_t *sem, int pshared, unsigned int value) {
  // Initialize lock.
  atomic_init(&sem->__lock.__state, CLOUDABI_LOCK_UNLOCKED);
  sem->__lock.__write_recursion = -1;
  sem->__lock.__pshared =
      pshared ? PTHREAD_PROCESS_SHARED : PTHREAD_PROCESS_PRIVATE;

  // Initialize condition variable.
  atomic_init(&sem->__cond.__waiters, CLOUDABI_CONDVAR_HAS_NO_WAITERS);
  sem->__cond.__clock = CLOUDABI_CLOCK_REALTIME;
  sem->__cond.__pshared =
      pshared ? PTHREAD_PROCESS_SHARED : PTHREAD_PROCESS_PRIVATE;

  // Initialize other fields.
  sem->__value = value;
  return 0;
}
