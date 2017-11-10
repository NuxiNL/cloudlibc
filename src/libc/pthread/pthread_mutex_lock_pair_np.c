// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>

int pthread_mutex_lock_pair_np(pthread_mutex_t *mutex1,
                               pthread_mutex_t *mutex2) {
  // Instead of determining the lock order based on the addresses of the
  // locks, use a 'smarter' algorithm that never blocks on a lock while
  // holding the other lock.
  //
  // The disadvantage of ordered locking is that it may cause a chain of
  // threads to be blocked on each other, whereas the 'smarter'
  // algorithm allows this chain to be broken. More details:
  //
  // http://howardhinnant.github.io/dining_philosophers.html
  for (;;) {
    pthread_mutex_lock(mutex1);
    if (pthread_mutex_trylock(mutex2) == 0)
      return 0;
    pthread_mutex_unlock(mutex1);

    pthread_mutex_lock(mutex2);
    if (pthread_mutex_trylock(mutex1) == 0)
      return 0;
    pthread_mutex_unlock(mutex2);
  }
}
