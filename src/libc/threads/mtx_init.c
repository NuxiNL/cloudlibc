// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>
#include <threads.h>

int mtx_init(mtx_t *mtx, int type) {
  pthread_mutexattr_t attr;
  if (pthread_mutexattr_init(&attr) != 0)
    return thrd_error;

  switch (type) {
    case mtx_plain:
    case mtx_timed:
      if (pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_NORMAL) != 0) {
        pthread_mutexattr_destroy(&attr);
        return thrd_error;
      }
      break;
    case mtx_plain | mtx_recursive:
    case mtx_timed | mtx_recursive:
      if (pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE) != 0) {
        pthread_mutexattr_destroy(&attr);
        return thrd_error;
      }
      break;
    default:
      pthread_mutexattr_destroy(&attr);
      return thrd_error;
  }

  if (pthread_mutex_init(mtx, &attr) != 0) {
    pthread_mutexattr_destroy(&attr);
    return thrd_error;
  }
  pthread_mutexattr_destroy(&attr);
  return thrd_success;
}
