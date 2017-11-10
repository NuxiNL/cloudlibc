// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <wchar.h>

int wcscasecmp_l(const wchar_t *ws1, const wchar_t *ws2, locale_t locale) {
  return wcscasecmp(ws1, ws2);
}
