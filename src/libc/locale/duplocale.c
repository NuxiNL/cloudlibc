// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>
#include <common/refcount.h>

#include <locale.h>

locale_t duplocale(locale_t locobj) {
  // Don't make an actual copy of the locale. It's sufficient to just
  // increase its reference count. This allows us to reuse the compiled
  // string pool.
  refcount_acquire(&locobj->refcount);
  return locobj;
}
