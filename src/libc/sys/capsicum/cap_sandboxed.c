// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/capsicum.h>

#include <stdbool.h>

bool cap_sandboxed(void) {
  // We're always in Capsicum mode.
  return true;
}
