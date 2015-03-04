// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <pthread.h>

int pthread_spin_init(pthread_spinlock_t *lock, int pshared) {
  return pthread_rwlock_init(lock, NULL);
}
