// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <pthread.h>
#include <threads.h>

int thrd_detach(thrd_t thr) {
  if (pthread_detach(thr) != 0)
    return thrd_error;
  return thrd_success;
}
