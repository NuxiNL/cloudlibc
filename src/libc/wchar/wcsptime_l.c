// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdbool.h>
#include <wchar.h>
#include <wctype.h>

// Parses one of the fixed strings in a list from the input.
static bool parse_string(const wchar_t *restrict *buf,
                         const wchar_t *const *strings, size_t strings_count,
                         int *result, locale_t locale) {
  for (size_t i = 0; i < strings_count; ++i) {
    const wchar_t *a = strings[i];
    const wchar_t *b = *buf;
    do {
      if (*a == L'\0') {
        *buf = b;
        *result = i;
        return true;
      }
    } while (towlower(*a++) == towlower(*b++));
  }
  return false;
}

// Parses one or more whitespace characters from the input.
static bool parse_whitespace(const wchar_t *restrict *buf, locale_t locale) {
  bool nonzero = false;
  while (iswspace(**buf)) {
    ++*buf;
    nonzero = true;
  }
  return nonzero;
}

#define NAME wcsptime_l
#define WIDE 1

#include <common/strptime.h>
