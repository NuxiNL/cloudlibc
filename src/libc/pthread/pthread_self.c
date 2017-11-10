// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/crt.h>

#include <pthread.h>
#include <threads.h>
#include <uv.h>

pthread_t pthread_self(void) {
  return __pthread_self_object;
}

__strong_reference(pthread_self, thrd_current);
__strong_reference(pthread_self, uv_thread_self);
