// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>
#include <threads.h>

void cnd_destroy(cnd_t *cond) {
  pthread_cond_destroy(cond);
}
