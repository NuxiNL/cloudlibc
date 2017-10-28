// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <uv.h>

int uv_tty_reset_mode(void) {
  // There is nothing that needs to be done in this environment, as
  // processes do not have a controlling TTY.
  return 0;
}
