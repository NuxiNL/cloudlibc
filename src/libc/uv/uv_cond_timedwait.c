// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <pthread.h>
#include <uv.h>

int uv_cond_timedwait(uv_cond_t *cond, uv_mutex_t *mutex, uint64_t timeout) {
  // uv_cond_timedwait() uses relative timeouts.
  timeout += uv_hrtime();
  struct timespec ts = {.tv_sec = timeout / 1000000000,
                        .tv_nsec = timeout % 1000000000};
  return -pthread_cond_timedwait(cond, mutex, &ts);
}
