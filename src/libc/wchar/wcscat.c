// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#define _CLOUDLIBC_UNSAFE_STRING_FUNCTIONS

#include <wchar.h>

wchar_t *__cloudlibc_wcscat(wchar_t *restrict ws1, const wchar_t *restrict ws2) {
  wchar_t *ws = ws1;
  while (*ws != L'\0')
    ++ws;
  for (;;) {
    *ws++ = *ws2;
    if (*ws2++ == L'\0')
      return ws1;
  }
}

__weak_reference(__cloudlibc_wcscat, wcscat);
