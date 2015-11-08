// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#define _CLOUDLIBC_UNSAFE_STRING_FUNCTIONS

#include <wchar.h>

wchar_t *wcsncat(wchar_t *restrict ws1, const wchar_t *restrict ws2, size_t n) {
  if (n > 0) {
    wchar_t *ws = ws1;
    while (*ws != L'\0')
      ++ws;
    do {
      *ws++ = *ws2;
      if (*ws2++ == L'\0')
        break;
    } while (--n > 0);
  }
  return ws1;
}
