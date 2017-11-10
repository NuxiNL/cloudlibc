// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>
#include <uv.h>

int uv_thread_join(uv_thread_t *tid) {
  return -pthread_join(*tid, NULL);
}
