// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>
#include <common/mbstate.h>

#include <stdbool.h>
#include <wctype.h>

// Parses one of the fixed strings in a list from the input.
static bool parse_string(const char *restrict *buf,
                         const wchar_t *const *strings, size_t strings_count,
                         int *result, locale_t locale) {
  for (size_t i = 0; i < strings_count; ++i) {
    const wchar_t *a = strings[i];
    const char *b = *buf;
    // Translate multibyte characters from the input to wide characters
    // before comparing.
    const struct lc_ctype *lc_ctype = locale->ctype;
    mbstate_t mbs;
    mbstate_set_init(&mbs);
    for (;;) {
      if (*a == L'\0') {
        *buf = b;
        *result = i;
        return true;
      }
      char32_t c32;
      ssize_t len = lc_ctype->mbtoc32(&c32, b, SIZE_MAX, &mbs, lc_ctype->data);
      if (len < 1)
        break;
      if (towlower(*a++) != towlower(c32))
        break;
      b += len;
    }
  }
  return false;
}

// Parses one or more whitespace characters from the input.
static bool parse_whitespace(const char *restrict *buf, locale_t locale) {
  bool nonzero = false;
  const struct lc_ctype *lc_ctype = locale->ctype;
  mbstate_t mbs;
  mbstate_set_init(&mbs);
  for (;;) {
    char32_t c32;
    ssize_t len = lc_ctype->mbtoc32(&c32, *buf, SIZE_MAX, &mbs, lc_ctype->data);
    if (len < 1 || !iswspace(c32))
      break;
    *buf += len;
    nonzero = true;
  }
  return nonzero;
}

#define NAME strptime_l
#define WIDE 0

#include <common/strptime.h>
