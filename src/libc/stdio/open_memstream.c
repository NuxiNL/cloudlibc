// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <locale.h>
#include <stdio.h>

FILE *open_memstream(char **bufp, size_t *sizep) {
  DEFAULT_LOCALE(locale, LC_CTYPE_MASK);
  return open_memstream_l(bufp, sizep, locale);
}
