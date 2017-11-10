// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <string.h>

#define MEMMEM 1
#define WIDE 0
#include <common/twoway.h>

void *(memmem)(const void *s1, size_t s1len, const void *s2, size_t s2len) {
  if (s2len > s1len)
    return NULL;
  if (s2len == 0)
    return (void *)s1;
  if (s2len == 1)
    return (void *)memchr(s1, *(const char *)s2, s1len);
  return (void *)twoway_memmem(s1, s1len, s2, s2len);
}
