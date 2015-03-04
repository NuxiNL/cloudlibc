// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/float.h>

#include <errno.h>
#include <wchar.h>

long double wcstold_l(const wchar_t *restrict nptr, wchar_t **restrict endptr,
                      locale_t locale) {
  struct float2 f2;
  if (!__wcstofloat2(nptr, endptr, locale, &f2)) {
    errno = EINVAL;
    return 0.0;
  }
  long double value;
  if (!__float2_to_long_double(&f2, &value))
    errno = ERANGE;
  return value;
}
