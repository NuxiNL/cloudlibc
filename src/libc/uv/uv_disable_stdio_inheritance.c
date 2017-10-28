// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <uv.h>

void uv_disable_stdio_inheritance(void) {
  // There is nothing that needs to be done in this environment, as file
  // descriptors cannot be inherited through close-on-exec flags.
}
