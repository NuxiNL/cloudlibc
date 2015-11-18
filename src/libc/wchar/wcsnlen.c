// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <wchar.h>

size_t wcsnlen(const wchar_t *ws, size_t maxlen) {
  const wchar_t *e = ws;
  while (maxlen-- > 0 && *e != L'\0')
    ++e;
  return e - ws;
}
