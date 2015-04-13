// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdatomic.h>
#include <stdlib.h>

#include "stdlib_impl.h"

int atexit(void (*func)(void)) {
  // Allocate new entry.
  struct atexit *entry = malloc(sizeof(*entry));
  if (entry == NULL)
    return -1;
  *entry = (struct atexit){.atexit = func};

  // Store it in the global list of atexit functions.
  entry->previous = atomic_load_explicit(&__atexit_last, memory_order_relaxed);
  while (!atomic_compare_exchange_weak_explicit(
             &__atexit_last, &entry->previous, entry, memory_order_release,
             memory_order_relaxed))
    ;
  return 0;
}
