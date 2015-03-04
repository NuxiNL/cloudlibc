// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

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
