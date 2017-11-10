// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <string.h>

void *(memrchr)(const void *s, int c, size_t n) {
  const unsigned char *sb = s;
  sb += n;
  while (n-- > 0) {
    --sb;
    if (*sb == (unsigned char)c)
      return (void *)sb;
  }
  return NULL;
}
