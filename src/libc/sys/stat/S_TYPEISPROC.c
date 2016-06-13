// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/stat.h>

#include <stdbool.h>

#ifndef S_TYPEISPROC
#error "S_TYPEISPROC is supposed to be a macro as well"
#endif

// clang-format off
bool (S_TYPEISPROC)(const struct stat *buf) {
  return S_TYPEISPROC(buf);
}
