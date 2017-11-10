// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/locale.h>

#include <stdlib.h>

size_t MB_CUR_MAX_L(locale_t locale) {
  return locale->ctype->c32tomb_max;
}
