// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wchar.h>

int wcsncmp(const wchar_t *ws1, const wchar_t *ws2, size_t n) {
  while (n-- > 0) {
    wchar_t c1 = *ws1++;
    wchar_t c2 = *ws2++;
    if (c1 < c2)
      return -1;
    if (c1 > c2)
      return 1;
    if (c1 == L'\0')
      break;
  }
  return 0;
}
