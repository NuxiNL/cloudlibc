// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/pthread.h>

#include <stdatomic.h>
#include <stdlib.h>

int __cxa_thread_atexit(void (*)(void *), void *, void *);

int __cxa_thread_atexit(void (*func)(void *), void *arg, void *dso_handle) {
  // Allocate new entry.
  struct thread_atexit *entry = malloc(sizeof(*entry));
  if (entry == NULL)
    return -1;
  *entry = (struct thread_atexit){.func = func, .arg = arg};

  // Store it in the global list of atexit functions.
  entry->previous =
      atomic_load_explicit(&__thread_atexit_last, memory_order_relaxed);
  while (!atomic_compare_exchange_weak_explicit(
      &__thread_atexit_last, &entry->previous, entry, memory_order_release,
      memory_order_relaxed))
    ;
  return 0;
}
