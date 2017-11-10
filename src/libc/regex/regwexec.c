// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <regex.h>
#include <wchar.h>

int regwexec(const regex_t *restrict preg, const wchar_t *restrict string,
             size_t nmatch, regmatch_t *restrict pmatch, int eflags) {
  return regwnexec(preg, string, wcslen(string), nmatch, pmatch, eflags);
}
