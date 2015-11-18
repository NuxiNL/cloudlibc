// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <wchar.h>

#define MEMMEM 0
#define WIDE 1
#include <common/twoway.h>

wchar_t *wcsstr(const wchar_t *restrict ws1, const wchar_t *restrict ws2) {
  size_t ws2len = wcslen(ws2);
  if (ws2len == 0)
    return (wchar_t *)ws1;
  if (ws2len == 1)
    return wcschr(ws1, *ws2);
  return (wchar_t *)twoway_strstr(ws1, ws2, ws2len);
}
