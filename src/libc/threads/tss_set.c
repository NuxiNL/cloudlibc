// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>
#include <threads.h>

int tss_set(tss_t key, void *val) {
  if (pthread_setspecific(key, val) != 0)
    return thrd_error;
  return thrd_success;
}
