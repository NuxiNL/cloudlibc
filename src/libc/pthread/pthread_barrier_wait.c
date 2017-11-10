// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>
#include <stdint.h>

int pthread_barrier_wait(pthread_barrier_t *barrier) {
  pthread_mutex_lock(&barrier->__lock);
  if (barrier->__remaining == 0) {
    // Last thread on the barrier. Reset the barrier and wake up waiters.
    barrier->__remaining = barrier->__init;
    ++barrier->__generation;
    pthread_mutex_unlock(&barrier->__lock);
    pthread_cond_broadcast(&barrier->__cond);
    return PTHREAD_BARRIER_SERIAL_THREAD;
  } else {
    // We have to wait for more threads. Go to sleep until we've reached
    // the next generation.
    --barrier->__remaining;
    uint32_t generation = barrier->__generation;
    do {
      pthread_cond_wait(&barrier->__cond, &barrier->__lock);
    } while (generation == barrier->__generation);
    pthread_mutex_unlock(&barrier->__lock);
    return 0;
  }
}
