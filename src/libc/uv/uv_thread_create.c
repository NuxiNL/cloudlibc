// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>
#include <uv.h>

int uv_thread_create(uv_thread_t *tid, uv_thread_cb entry, void *arg) {
  return -pthread_create(tid, NULL, (void *(*)(void *))entry, arg);
}
