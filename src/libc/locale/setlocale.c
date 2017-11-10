// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <locale.h>
#include <string.h>

char *setlocale(int category, const char *locale) {
  // This environment doesn't allow us to override the process global
  // locale. It is always set to "C". As there is a lot of code out
  // there that uses this function either to query the current locale or
  // to temporarily switch to the "C" locale, provide this simple stub.
  return locale == NULL || *locale == '\0' || strcmp(locale, "C") == 0 ||
                 strcmp(locale, "POSIX") == 0
             ? (char *)"C"
             : NULL;
}
