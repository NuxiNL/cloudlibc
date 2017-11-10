// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/crt.h>
#include <common/refcount.h>

#include <pthread.h>

int pthread_detach(pthread_t thread) {
  if (refcount_release(&thread->refcount)) {
    // Thread has already passed the point that allows us to detach.
    // Join the thread itself to ensure the thread's resources are
    // cleaned up.
    return pthread_join(thread, NULL);
  }
  return 0;
}
