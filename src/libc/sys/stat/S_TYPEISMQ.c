// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/stat.h>

#include <stdbool.h>

#ifndef S_TYPEISMQ
#error "S_TYPEISMQ is supposed to be a macro as well"
#endif

// clang-format off
bool (S_TYPEISMQ)(const struct stat *buf) {
  return S_TYPEISMQ(buf);
}
