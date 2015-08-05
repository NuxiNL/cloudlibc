// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdatomic.h>
#include <stdlib.h>

#include "stdlib_impl.h"

int __cxa_atexit(void (*)(void *), void *, void *);

int __cxa_atexit(void (*func)(void *), void *arg, void *dso_handle) {
  // Allocate new entry.
  struct atexit *entry = malloc(sizeof(*entry));
  if (entry == NULL)
    return -1;
  *entry = (struct atexit){.cxa_atexit = func, .cxa_atexit_arg = arg};

  // Store it in the global list of atexit functions.
  entry->previous = atomic_load_explicit(&__atexit_last, memory_order_relaxed);
  while (!atomic_compare_exchange_weak_explicit(
      &__atexit_last, &entry->previous, entry, memory_order_release,
      memory_order_relaxed))
    ;
  return 0;
}
