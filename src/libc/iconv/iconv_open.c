// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <errno.h>
#include <iconv.h>
#include <stdlib.h>
#include <string.h>

#include "iconv_impl.h"

iconv_t iconv_open(const char *tocode, const char *fromcode) {
  // Parse source character set name.
  const struct lc_ctype *from = __lookup_ctype(fromcode, strlen(fromcode));
  if (from == NULL) {
    errno = EINVAL;
    return (iconv_t)-1;
  }

  // Parse "//IGNORE" suffix in target character set name.
  static const char suffix[] = "//IGNORE";
  size_t suffixlen = sizeof(suffix) - 1;
  size_t tolen = strlen(tocode);
  bool ignore = false;
  if (tolen >= suffixlen && strcmp(tocode + tolen - suffixlen, suffix) == 0) {
    ignore = true;
    tolen -= suffixlen;
  }

  // Parse target character set name.
  const struct lc_ctype *to = __lookup_ctype(tocode, tolen);
  if (to == NULL) {
    errno = EINVAL;
    return (iconv_t)-1;
  }

  // Allocate iconv handle.
  iconv_t cd = calloc(1, sizeof(*cd));
  if (cd == NULL)
    return (iconv_t)-1;
  cd->from = from;
  cd->to = to;
  cd->ignore = ignore;
  return cd;
}
