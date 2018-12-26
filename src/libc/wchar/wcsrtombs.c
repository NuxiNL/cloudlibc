// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <locale.h>
#include <stdint.h>
#include <wchar.h>
#include <cloudlibc_interceptors.h>

size_t __cloudlibc_wcsrtombs(char *restrict dst, const wchar_t **restrict src, size_t len,
                 mbstate_t *restrict ps) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK);
  return wcsnrtombs_l(dst, src, SIZE_MAX, len, ps, locale);
}

__weak_reference(__cloudlibc_wcsrtombs, wcsrtombs);
