// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>
#include <uv.h>

void *uv_key_get(uv_key_t *key) {
  return pthread_getspecific(*key);
}
