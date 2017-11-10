// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wctype.h>

wctype_t wctype_l(const char *property, locale_t locale) {
  return wctype(property);
}
