// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <locale.h>
#include <stdint.h>
#include <stdlib.h>
#include <wchar.h>
#include <cloudlibc_interceptors.h>

size_t __cloudlibc_wcstombs(char *restrict s, const wchar_t *restrict pwcs, size_t n) {
  const wchar_t *src = pwcs;
  static const mbstate_t initial_mbstate;
  mbstate_t ps = initial_mbstate;
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK);
  return wcsnrtombs_l(s, &src, SIZE_MAX, n, &ps, locale);
}

__weak_reference(__cloudlibc_wcstombs, wcstombs);
