// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <pthread.h>
#include <uv.h>

int uv_thread_create(uv_thread_t *tid, uv_thread_cb entry, void *arg) {
  return -pthread_create(tid, NULL, (void *(*)(void *))entry, arg);
}
