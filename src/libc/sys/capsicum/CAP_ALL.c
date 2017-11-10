// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/capsicum.h>

#ifndef CAP_ALL
#error "CAP_ALL is supposed to be a macro as well"
#endif

// clang-format off
void (CAP_ALL)(cap_rights_t *rights) {
  CAP_ALL(rights);
}
