// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/capsicum.h>

#include <stdbool.h>

bool cap_sandboxed(void) {
  // We're always in Capsicum mode.
  return true;
}
