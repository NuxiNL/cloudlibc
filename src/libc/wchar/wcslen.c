// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wchar.h>
#include <cloudlibc_interceptors.h>

size_t __cloudlibc_wcslen(const wchar_t *ws) {
  const wchar_t *e = ws;
  while (*e != L'\0')
    ++e;
  return e - ws;
}

__weak_reference(__cloudlibc_wcslen, wcslen);
