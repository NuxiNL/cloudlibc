// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <string.h>
#include <wchar.h>

wchar_t *wmemcpy(wchar_t *restrict ws1, const wchar_t *restrict ws2, size_t n) {
  wchar_t *b = ws1;
  while (n-- > 0)
    *ws1++ = *ws2++;
  return b;
}
