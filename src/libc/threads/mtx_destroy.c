// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>
#include <threads.h>

void mtx_destroy(mtx_t *mtx) {
  pthread_mutex_destroy(mtx);
}
