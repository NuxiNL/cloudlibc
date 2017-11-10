// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wchar.h>

wchar_t *wmemset(wchar_t *ws, wchar_t wc, size_t n) {
  wchar_t *wsb = ws;
  while (n-- > 0)
    *wsb++ = wc;
  return ws;
}
