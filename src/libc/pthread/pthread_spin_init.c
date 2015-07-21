// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalldefs.h>

#include <pthread.h>
#include <stdatomic.h>

int pthread_spin_init(pthread_spinlock_t *lock, int pshared) {
  atomic_init(&lock->__state, CLOUDABI_LOCK_UNLOCKED);
  lock->__write_recursion = -1;
  lock->__pshared = pshared ? PTHREAD_PROCESS_SHARED : PTHREAD_PROCESS_PRIVATE;
  return 0;
}
