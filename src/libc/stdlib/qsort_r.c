// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdlib.h>

#ifndef qsort_r
#error "qsort_r is supposed to be a macro as well"
#endif

// clang-format off
void (qsort_r)(void *base, size_t nel, size_t width,
               int (*compar)(const void *, const void *, void *), void *thunk) {
  return qsort_r(base, nel, width, compar, thunk);
}
