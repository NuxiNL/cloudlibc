// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wchar.h>

size_t wcsnlen(const wchar_t *ws, size_t maxlen) {
  const wchar_t *e = ws;
  while (maxlen-- > 0 && *e != L'\0')
    ++e;
  return e - ws;
}
