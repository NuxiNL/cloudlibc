// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wchar.h>

size_t wcslen(const wchar_t *ws) {
  const wchar_t *e = ws;
  while (*e != L'\0')
    ++e;
  return e - ws;
}
