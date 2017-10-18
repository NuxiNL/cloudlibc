// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <pthread.h>
#include <uv.h>

void *uv_key_get(uv_key_t *key) {
  return pthread_getspecific(*key);
}
