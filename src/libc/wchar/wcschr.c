// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <wchar.h>

wchar_t *wcschr(const wchar_t *ws, wchar_t wc) {
  for (;;) {
    if (*ws == wc)
      return (wchar_t *)ws;
    if (*ws++ == L'\0')
      return NULL;
  }
}
