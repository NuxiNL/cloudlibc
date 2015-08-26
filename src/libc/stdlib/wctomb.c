
// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <limits.h>
#include <stdlib.h>

int wctomb(char *s, wchar_t wchar) {
  if (s == NULL)
    return 0;
  if (wchar < 0 || wchar > SCHAR_MAX) {
    errno = EILSEQ;
    return -1;
  }
  *s = wchar;
  return 1;
}
