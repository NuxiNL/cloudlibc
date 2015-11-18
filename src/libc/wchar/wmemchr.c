// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <wchar.h>

wchar_t *wmemchr(const wchar_t *ws, wchar_t wc, size_t n) {
  while (n-- > 0) {
    if (*ws == wc)
      return (wchar_t *)ws;
    ++ws;
  }
  return NULL;
}
