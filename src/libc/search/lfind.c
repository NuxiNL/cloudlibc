// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <search.h>

#ifndef lfind
#error "lfind is supposed to be a macro as well"
#endif

void *(lfind)(const void *key, const void *base, size_t *nelp, size_t width,
              int (*compar)(const void *, const void *)) {
  return (void *)lfind(key, base, nelp, width, compar);
}
