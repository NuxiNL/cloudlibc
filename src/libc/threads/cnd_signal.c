// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>
#include <threads.h>

int cnd_signal(cnd_t *cond) {
  if (pthread_cond_signal(cond) != 0)
    return thrd_error;
  return thrd_success;
}
