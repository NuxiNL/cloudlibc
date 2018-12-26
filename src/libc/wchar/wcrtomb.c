// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <locale.h>
#include <uchar.h>
#include <wchar.h>
#include <cloudlibc_interceptors.h>

size_t __cloudlibc_wcrtomb(char *restrict s, wchar_t wc, mbstate_t *restrict ps) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK);
  return c32rtomb_l(s, wc, ps, locale);
}

__weak_reference(__cloudlibc_wcrtomb, wcrtomb);
