// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <locale.h>
#include <stdint.h>
#include <wchar.h>
#include <cloudlibc_interceptors.h>

size_t __cloudlibc_mbsrtowcs(wchar_t *restrict dst, const char **restrict src, size_t len,
                 mbstate_t *restrict ps) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK);
  return mbsnrtowcs_l(dst, src, SIZE_MAX, len, ps, locale);
}

__weak_reference(__cloudlibc_mbsrtowcs, mbsrtowcs);
