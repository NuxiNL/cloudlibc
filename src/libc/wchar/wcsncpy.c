// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wchar.h>

wchar_t *wcsncpy(wchar_t *restrict ws1, const wchar_t *restrict ws2, size_t n) {
  wchar_t *begin = ws1;
  while (n > 0 && *ws2 != L'\0') {
    *ws1++ = *ws2++;
    --n;
  }
  while (n-- > 0)
    *ws1++ = L'\0';
  return begin;
}
