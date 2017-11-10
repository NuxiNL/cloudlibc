// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>
#include <uv.h>

int uv_thread_equal(const uv_thread_t *t1, const uv_thread_t *t2) {
  return pthread_equal(*t1, *t2);
}
