// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <wchar.h>

wchar_t *wmemset(wchar_t *ws, wchar_t wc, size_t n) {
  wchar_t *wsb = ws;
  while (n-- > 0)
    *wsb++ = wc;
  return ws;
}
