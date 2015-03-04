// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/pthread.h>

#include <errno.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdlib.h>

int pthread_atfork(void (*prepare)(void), void (*parent)(void),
                   void (*child)(void)) {
  // Entry would have no effect.
  if (prepare == NULL && parent == NULL && child == NULL)
    return 0;

  // Allocate new entry.
  struct pthread_atfork *entry = malloc(sizeof(*entry));
  if (entry == NULL)
    return ENOMEM;
  *entry = (struct pthread_atfork){
      .prepare = prepare, .parent = parent, .child = child,
  };

  // Store it in the global list of fork handlers.
  entry->previous =
      atomic_load_explicit(&__pthread_atfork_last, memory_order_relaxed);
  while (!atomic_compare_exchange_weak_explicit(
             &__pthread_atfork_last, &entry->previous, entry,
             memory_order_release, memory_order_relaxed))
    ;

  return 0;
}
