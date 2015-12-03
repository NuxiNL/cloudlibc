// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <search.h>

#ifndef lfind
#error "lfind is supposed to be a macro as well"
#endif

void *(lfind)(const void *key, const void *base, size_t *nelp, size_t width,
              int (*compar)(const void *, const void *)) {
  return lfind(key, base, nelp, width, compar);
}
