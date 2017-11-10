// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wchar.h>
#include <wctype.h>

int wcsncasecmp(const wchar_t *ws1, const wchar_t *ws2, size_t n) {
  while (n-- > 0) {
    wchar_t c1 = towlower(*ws1++);
    wchar_t c2 = towlower(*ws2++);
    if (c1 < c2)
      return -1;
    if (c1 > c2)
      return 1;
    if (c1 == L'\0')
      break;
  }
  return 0;
}
