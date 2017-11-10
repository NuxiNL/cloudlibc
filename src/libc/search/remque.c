// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <search.h>

#ifndef remque
#error "remque is supposed to be a macro as well"
#endif

// clang-format off
void (remque)(void *element) {
  remque(element);
}
