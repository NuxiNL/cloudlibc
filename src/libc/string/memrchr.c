// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <string.h>
#include <cloudlibc_interceptors.h>

void *__cloudlibc_memrchr(const void *s, int c, size_t n) {
  const unsigned char *sb = s;
  sb += n;
  while (n-- > 0) {
    --sb;
    if (*sb == (unsigned char)c)
      return (void *)sb;
  }
  return NULL;
}

__weak_reference(__cloudlibc_memrchr, memrchr);
