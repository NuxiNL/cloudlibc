// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <pthread.h>
#include <stdlib.h>
#include <uv.h>

void uv_once(uv_once_t *guard, void (*callback)(void)) {
  if (pthread_once(guard, callback) != 0)
    abort();
}
