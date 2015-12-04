// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <search.h>

#ifndef lsearch
#error "lsearch is supposed to be a macro as well"
#endif

void *(lsearch)(const void *key, void *base, size_t *nelp, size_t width,
                int (*compar)(const void *, const void *)) {
  return lsearch(key, base, nelp, width, compar);
}
