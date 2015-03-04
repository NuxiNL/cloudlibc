// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/float.h>

#include <errno.h>
#include <stdlib.h>

double strtod_l(const char *restrict nptr, char **restrict endptr,
                locale_t locale) {
  struct float2 f2;
  if (!__strtofloat2(nptr, endptr, locale, &f2)) {
    errno = EINVAL;
    return 0.0;
  }
  double value;
  if (!__float2_to_double(&f2, &value))
    errno = ERANGE;
  return value;
}
