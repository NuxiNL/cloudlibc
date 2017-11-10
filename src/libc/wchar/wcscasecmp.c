// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wchar.h>
#include <wctype.h>

int wcscasecmp(const wchar_t *ws1, const wchar_t *ws2) {
  for (;;) {
    wint_t c1 = towlower(*ws1++);
    wint_t c2 = towlower(*ws2++);
    if (c1 < c2)
      return -1;
    if (c1 > c2)
      return 1;
    if (c1 == L'\0')
      return 0;
  }
}
