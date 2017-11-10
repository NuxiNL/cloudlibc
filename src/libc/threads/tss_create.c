// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <assert.h>
#include <limits.h>
#include <pthread.h>
#include <threads.h>

static_assert(TSS_DTOR_ITERATIONS == PTHREAD_DESTRUCTOR_ITERATIONS,
              "Value mismatch");

int tss_create(tss_t *key, tss_dtor_t dtor) {
  if (pthread_key_create(key, dtor) != 0)
    return thrd_error;
  return thrd_success;
}
