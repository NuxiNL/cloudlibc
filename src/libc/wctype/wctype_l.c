// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <wctype.h>

wctype_t wctype_l(const char *property, locale_t locale) {
  return wctype(property);
}
