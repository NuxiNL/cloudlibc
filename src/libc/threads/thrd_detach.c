// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>
#include <threads.h>

int thrd_detach(thrd_t thr) {
  if (pthread_detach(thr) != 0)
    return thrd_error;
  return thrd_success;
}
