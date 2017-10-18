// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <pthread.h>
#include <stdlib.h>
#include <uv.h>

void uv_key_delete(uv_key_t *key) {
  if (pthread_key_delete(*key) != 0)
    abort();
}
