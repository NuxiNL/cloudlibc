// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <ctype.h>

int iscntrl(int c) {
  return (c >= '\0' && c < ' ') || c == 0x7f;
}
