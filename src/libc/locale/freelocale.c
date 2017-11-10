// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>
#include <common/refcount.h>

#include <locale.h>
#include <stdatomic.h>
#include <stdlib.h>

void freelocale(locale_t locobj) {
  // Decrement reference count.
  if (refcount_release(&locobj->refcount)) {
    // Free compiled string object. Assume all of its members can be
    // passed to free().
    if (locobj->compiled != NULL) {
      struct lc_compiled *compiled = locobj->compiled;
      _Atomic(void *) *end = (void *)(compiled + 1);
      for (_Atomic(void *) *string = (void *)compiled; string < end; ++string)
        free(atomic_load_explicit(string, memory_order_relaxed));
      free(compiled);
    }

    // Free locale object itself.
    free(locobj);
  }
}
