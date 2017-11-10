// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <locale.h>
#include <regex.h>

#if WIDE
#include <wchar.h>
typedef wchar_t char_t;
#else
typedef char char_t;
#endif

#if WIDE
int NAME(const regex_t *restrict preg, const char_t *restrict string,
         size_t len, size_t nmatch, regmatch_t *restrict pmatch, int eflags) {
#else
int NAME(const regex_t *restrict preg, const char_t *restrict string,
         size_t len, size_t nmatch, regmatch_t *restrict pmatch, int eflags,
         locale_t locale) {
#endif
  return REG_NOMATCH;
}
