// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <cloudabi_types.h>
#include <pthread.h>
#include <stdatomic.h>

int pthread_spin_init(pthread_spinlock_t *lock, int pshared) {
  atomic_init(&lock->__state, CLOUDABI_LOCK_UNLOCKED);
  lock->__write_recursion = -1;
  lock->__pshared = pshared;
  return 0;
}
