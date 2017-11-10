// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>
#include <stdlib.h>
#include <uv.h>

void uv_key_delete(uv_key_t *key) {
  if (pthread_key_delete(*key) != 0)
    abort();
}
