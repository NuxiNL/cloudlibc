// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wchar.h>

wchar_t *(wcschr)(const wchar_t *ws, wchar_t wc) {
  for (;;) {
    if (*ws == wc)
      return (wchar_t *)ws;
    if (*ws++ == L'\0')
      return NULL;
  }
}
