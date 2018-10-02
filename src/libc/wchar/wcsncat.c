// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#define _CLOUDLIBC_UNSAFE_STRING_FUNCTIONS

#include <wchar.h>
#include <cloudlibc_interceptors.h>

wchar_t *__cloudlibc_wcsncat(wchar_t *restrict ws1, const wchar_t *restrict ws2, size_t n) {
  if (n > 0) {
    wchar_t *ws = ws1;
    while (*ws != L'\0')
      ++ws;
    do {
      if (*ws2 == L'\0')
        break;
      *ws++ = *ws2++;
    } while (--n > 0);
    *ws = L'\0';
  }
  return ws1;
}

__weak_reference(__cloudlibc_wcsncat, wcsncat);
