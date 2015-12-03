// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <search.h>

#ifndef insque
#error "insque is supposed to be a macro as well"
#endif

// clang-format off
void (insque)(void *element, void *pred) {
  insque(element, pred);
}
