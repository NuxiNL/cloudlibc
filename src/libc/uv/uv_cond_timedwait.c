// Copyright (c) 2017-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>
#include <uv.h>

int uv_cond_timedwait(uv_cond_t *cond, uv_mutex_t *mutex, uint64_t timeout) {
  struct timespec ts = {.tv_sec = timeout / 1000000000,
                        .tv_nsec = timeout % 1000000000};
  return -pthread_cond_timedwait_relative_np(cond, mutex, &ts);
}
