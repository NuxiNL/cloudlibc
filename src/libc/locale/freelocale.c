// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

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
      _Atomic(void *)*end = (void *)(compiled + 1);
      for (_Atomic(void *)*string = (void *)compiled; string < end; ++string)
        free(atomic_load_explicit(string, memory_order_relaxed));
      free(compiled);
    }

    // Free locale object itself.
    free(locobj);
  }
}
