// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/capsicum.h>

int cap_enter(void) {
  // We're always in Capsicum mode.
  return 0;
}
