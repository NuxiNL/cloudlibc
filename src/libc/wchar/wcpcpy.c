// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#define _CLOUDLIBC_UNSAFE_STRING_FUNCTIONS

#include <wchar.h>

wchar_t *wcpcpy(wchar_t *restrict ws1, const wchar_t *restrict ws2) {
  for (;;) {
    *ws1 = *ws2;
    if (*ws2++ == L'\0')
      return ws1;
    ++ws1;
  }
}
