// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <locale.h>
#include <regex.h>
#include <wchar.h>

struct inputbuf {
  const wchar_t *buf;
  size_t len;
};

#include "regex_compile.h"

int regwncomp_l(regex_t *restrict preg, const wchar_t *restrict pattern,
                size_t len, int cflags, locale_t locale) {
  struct inputbuf buf = {
      .buf = pattern,
      .len = len,
  };
  return regex_compile(preg, &buf, cflags, locale);
}
