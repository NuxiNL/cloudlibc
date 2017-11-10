// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>
#include <stdlib.h>
#include <uv.h>

void uv_once(uv_once_t *guard, void (*callback)(void)) {
  if (pthread_once(guard, callback) != 0)
    abort();
}
