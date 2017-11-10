// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <locale.h>
#include <wchar.h>

float wcstof(const wchar_t *restrict nptr, wchar_t **restrict endptr) {
  DEFAULT_LOCALE(locale, LC_NUMERIC_MASK);
  return wcstof_l(nptr, endptr, locale);
}
