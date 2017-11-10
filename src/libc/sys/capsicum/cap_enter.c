// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/capsicum.h>

int cap_enter(void) {
  // We're always in Capsicum mode.
  return 0;
}
