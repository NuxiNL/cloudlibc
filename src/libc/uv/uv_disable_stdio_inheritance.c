// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <uv.h>

void uv_disable_stdio_inheritance(void) {
  // There is nothing that needs to be done in this environment, as file
  // descriptors cannot be inherited through close-on-exec flags.
}
