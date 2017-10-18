// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/crt.h>

#include <pthread.h>
#include <threads.h>
#include <uv.h>

pthread_t pthread_self(void) {
  return __pthread_self_object;
}

__strong_reference(pthread_self, thrd_current);
__strong_reference(pthread_self, uv_thread_self);
