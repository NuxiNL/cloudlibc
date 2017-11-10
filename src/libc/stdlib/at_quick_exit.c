// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdatomic.h>
#include <stdlib.h>

#include "stdlib_impl.h"

int at_quick_exit(void (*func)(void)) {
  // Allocate new entry.
  struct at_quick_exit *entry = malloc(sizeof(*entry));
  if (entry == NULL)
    return -1;
  *entry = (struct at_quick_exit){.func = func};

  // Store it in the global list of at_quick_exit functions.
  entry->previous =
      atomic_load_explicit(&__at_quick_exit_last, memory_order_relaxed);
  while (!atomic_compare_exchange_weak_explicit(
      &__at_quick_exit_last, &entry->previous, entry, memory_order_release,
      memory_order_relaxed))
    ;
  return 0;
}
