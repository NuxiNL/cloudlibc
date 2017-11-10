// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>
#include <uv.h>

int uv_mutex_init_recursive(uv_mutex_t *handle) {
  pthread_mutexattr_t attr;
  int error = pthread_mutexattr_init(&attr);
  if (error != 0)
    return -error;
  error = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
  if (error != 0) {
    pthread_mutexattr_destroy(&attr);
    return -error;
  }
  error = pthread_mutex_init(handle, &attr);
  pthread_mutexattr_destroy(&attr);
  return -error;
}
