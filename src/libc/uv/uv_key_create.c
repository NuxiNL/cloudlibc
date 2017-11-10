// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>
#include <uv.h>

int uv_key_create(uv_key_t *key) {
  return -pthread_key_create(key, NULL);
}
