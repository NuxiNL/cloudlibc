// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <locale.h>
#include <regex.h>

struct inputbuf {
  const struct lc_ctype *ctype;
  mbstate_t mbs;
  const char *buf;
  size_t len;
};

#include "regex_compile.h"

int regncomp_l(regex_t *restrict preg, const char *restrict pattern, size_t len,
               int cflags, locale_t locale) {
  struct inputbuf buf = {
      .ctype = locale->ctype,
      .buf = pattern,
      .len = len,
  };
  return regex_compile(preg, &buf, cflags, locale);
}
