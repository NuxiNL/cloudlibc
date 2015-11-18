// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <pthread.h>
#include <threads.h>

void mtx_destroy(mtx_t *mtx) {
  pthread_mutex_destroy(mtx);
}
