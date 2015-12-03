// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <search.h>

#ifndef remque
#error "remque is supposed to be a macro as well"
#endif

// clang-format off
void (remque)(void *element) {
  remque(element);
}
