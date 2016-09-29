// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <string.h>

#include "aeabi_impl.h"

#ifdef __arm__

void __aeabi_memclr(void *s, size_t n) {
  memset(s, '\0', n);
}

__strong_reference(__aeabi_memclr, __aeabi_memclr4);
__strong_reference(__aeabi_memclr, __aeabi_memclr8);

#endif
