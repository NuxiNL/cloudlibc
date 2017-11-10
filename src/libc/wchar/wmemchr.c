// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wchar.h>

wchar_t *(wmemchr)(const wchar_t *ws, wchar_t wc, size_t n) {
  while (n-- > 0) {
    if (*ws == wc)
      return (wchar_t *)ws;
    ++ws;
  }
  return NULL;
}
