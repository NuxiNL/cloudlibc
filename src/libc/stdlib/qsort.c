// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdlib.h>

#ifndef qsort
#error "qsort is supposed to be a macro as well"
#endif

// clang-format off
void (qsort)(void *base, size_t nel, size_t width,
             int (*compar)(const void *, const void *)) {
  return qsort(base, nel, width, compar);
}
