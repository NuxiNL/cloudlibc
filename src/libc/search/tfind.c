// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <search.h>

#ifndef tfind
#error "tfind is supposed to be a macro as well"
#endif

posix_tnode *(tfind)(const void *key, posix_tnode *const *rootp,
                     int (*compar)(const void *, const void *)) {
  return tfind(key, rootp, compar);
}
