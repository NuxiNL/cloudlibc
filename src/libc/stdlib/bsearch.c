// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdlib.h>

#ifndef bsearch
#error "bsearch is supposed to be a macro as well"
#endif

void *(bsearch)(const void *key, const void *base, size_t nel, size_t width,
                int (*compar)(const void *, const void *)) {
  return (void *)bsearch(key, base, nel, width, compar);
}
