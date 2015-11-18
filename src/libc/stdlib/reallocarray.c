// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/overflow.h>

#include <errno.h>
#include <stdlib.h>

void *reallocarray(void *ptr, size_t nelem, size_t elsize) {
  // Check for overflow of nelem * elsize.
  size_t size;
  if (mul_overflow(nelem, elsize, &size)) {
    errno = ENOMEM;
    return NULL;
  }
  return realloc(ptr, size);
}
