// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <ctype.h>

int isprint(int c) {
  return c >= ' ' && c <= '~';
}
