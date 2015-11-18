// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <pthread.h>
#include <stdint.h>
#include <threads.h>

int thrd_join(thrd_t thr, int *res) {
  void *retval;
  if (pthread_join(thr, &retval) != 0)
    return thrd_error;
  if (res != NULL)
    *res = (intptr_t)retval;
  return thrd_success;
}
