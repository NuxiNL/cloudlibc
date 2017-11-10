// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/capsicum.h>

#ifndef CAP_NONE
#error "CAP_NONE is supposed to be a macro as well"
#endif

// clang-format off
void (CAP_NONE)(cap_rights_t *rights) {
  CAP_NONE(rights);
}
