// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#define _CLOUDLIBC_UNSAFE_STRING_FUNCTIONS

#include <wchar.h>

wchar_t *wcscpy(wchar_t *restrict ws1, const wchar_t *restrict ws2) {
  wchar_t *ws = ws1;
  for (;;) {
    *ws++ = *ws2;
    if (*ws2++ == L'\0')
      return ws1;
  }
}
