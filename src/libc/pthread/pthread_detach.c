// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/crt.h>

#include <pthread.h>
#include <stdatomic.h>

int pthread_detach(pthread_t thread) {
  // Set thread detached bit.
  unsigned int old = atomic_fetch_or_explicit(
      &thread->detachstate, DETACH_DETACHED, memory_order_relaxed);
  assert((old & DETACH_DETACHED) == 0 && "Attempted to detach thread twice");
  if ((old & DETACH_TERMINATING) != 0) {
    // Thread has already passed the point that allows us to detach.
    // Join the thread itself to ensure the thread's resources are
    // cleaned up.
    return pthread_join(thread, NULL);
  }
  return 0;
}
