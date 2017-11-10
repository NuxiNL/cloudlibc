// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wchar.h>

int wmemcmp(const wchar_t *ws1, const wchar_t *ws2, size_t n) {
  while (n-- > 0) {
    if (*ws1 < *ws2)
      return -1;
    if (*ws1 > *ws2)
      return 1;
    ++ws1;
    ++ws2;
  }
  return 0;
}
