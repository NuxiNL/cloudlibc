// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <uv.h>

int uv_tty_reset_mode(void) {
  // There is nothing that needs to be done in this environment, as
  // processes do not have a controlling TTY.
  return 0;
}
