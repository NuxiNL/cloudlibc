// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/limits.h>
#include <common/pthread.h>
#include <common/syscalls.h>

#include <pthread.h>
#include <stdatomic.h>
#include <stdint.h>

int pthread_cond_broadcast(pthread_cond_t *cond) {
  if (atomic_load_explicit(&cond->__waiters, memory_order_relaxed) !=
      CLOUDABI_CONDVAR_HAS_NO_WAITERS) {
    cloudabi_errno_t error = cloudabi_sys_condvar_signal(
        &cond->__waiters, cond->__pshared, NUMERIC_MAX(cloudabi_nthreads_t));
    if (error != 0)
      __pthread_terminate(error, "Failed to broadcast on condition variable");
  }
  return 0;
}
