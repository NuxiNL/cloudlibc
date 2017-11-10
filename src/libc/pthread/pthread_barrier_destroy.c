// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <assert.h>
#include <pthread.h>

int pthread_barrier_destroy(pthread_barrier_t *barrier) {
  assert(barrier->__remaining == barrier->__init &&
         "Barrier destroyed with threads waiting");

  pthread_mutex_destroy(&barrier->__lock);
  pthread_cond_destroy(&barrier->__cond);
  return 0;
}
