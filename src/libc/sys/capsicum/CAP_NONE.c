// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/capsicum.h>

#ifndef CAP_NONE
#error "CAP_NONE is supposed to be a macro as well"
#endif

// clang-format off
void (CAP_NONE)(cap_rights_t *rights) {
  CAP_NONE(rights);
}
