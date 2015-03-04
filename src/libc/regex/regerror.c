// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <locale.h>
#include <regex.h>

size_t regerror(int errcode, const regex_t *restrict preg,
                char *restrict errbuf, size_t errbuf_size) {
  return regerror_l(errcode, preg, errbuf, errbuf_size, LC_GLOBAL_LOCALE);
}
