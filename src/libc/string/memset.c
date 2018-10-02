// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <cloudlibc_interceptors.h>
#include <limits.h>
#include <string.h>

#include "string_impl.h"

void *__cloudlibc_memset(void *s, int c, size_t n) {
  char *sb = s;
  if (n >= LONG_STRING_SIZE) {
    // Set first bytes until buffer is aligned to unsigned long.
    while (!is_long_aligned(sb)) {
      *sb++ = c;
      --n;
    }

    // Set multiple characters per iteration.
    unsigned long cl = construct_chars(c);
    unsigned long *sl = (unsigned long *)sb;
    do {
      *sl++ = cl;
      n -= sizeof(unsigned long);
    } while (n >= sizeof(unsigned long));
    sb = (char *)sl;
  }

  // Switch back to setting individual characters for the last couple of
  // bytes.
  while (n-- > 0)
    *sb++ = c;
  return s;
}

__weak_reference(__cloudlibc_memset, memset);
