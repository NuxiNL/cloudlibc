// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wchar.h>

#define MEMMEM 1
#define WIDE 1
#include <common/twoway.h>

wchar_t *(wmemmem)(const wchar_t *ws1, size_t ws1len, const wchar_t *ws2,
                   size_t ws2len) {
  if (ws2len > ws1len)
    return NULL;
  if (ws2len == 0)
    return (wchar_t *)ws1;
  if (ws2len == 1)
    return (wchar_t *)wmemchr(ws1, *ws2, ws1len);
  return (wchar_t *)twoway_memmem(ws1, ws1len, ws2, ws2len);
}
