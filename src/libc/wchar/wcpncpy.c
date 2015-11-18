// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <wchar.h>

wchar_t *wcpncpy(wchar_t *restrict ws1, const wchar_t *restrict ws2, size_t n) {
  while (n > 0 && *ws2 != L'\0') {
    *ws1++ = *ws2++;
    --n;
  }
  wchar_t *end = ws1;
  while (n-- > 0)
    *ws1++ = L'\0';
  return end;
}
