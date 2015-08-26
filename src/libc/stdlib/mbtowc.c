// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <stdlib.h>

int mbtowc(wchar_t *restrict pwc, const char *restrict s, size_t n) {
  if (s == NULL)
    return 0;
  if (n < 1 || *s < 0) {
    errno = EILSEQ;
    return -1;
  }
  if (pwc != NULL)
    *pwc = *s;
  return *s != '\0';
}
