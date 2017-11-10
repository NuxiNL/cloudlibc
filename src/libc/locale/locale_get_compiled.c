// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <stdatomic.h>
#include <stdlib.h>

// Obtains the compiled data structure stored within a locale. If no
// compiled data structure is available, it will attempt to allocate a
// new one.
struct lc_compiled *__locale_get_compiled(locale_t locale) {
  struct lc_compiled *compiled = atomic_load(&locale->compiled);
  if (compiled == NULL) {
    struct lc_compiled *new_compiled = calloc(1, sizeof(*compiled));
    if (new_compiled == NULL)
      return NULL;

    // Perform a compare-and-exchange to put the new compiled data
    // structure in place. If this fails, another thread must have
    // created a new structure faster than us. Use that copy instead.
    if (atomic_compare_exchange_strong(&locale->compiled, &compiled,
                                       new_compiled)) {
      compiled = new_compiled;
    } else {
      free(new_compiled);
    }
  }
  return compiled;
}
