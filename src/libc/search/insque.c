// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <search.h>

#ifndef insque
#error "insque is supposed to be a macro as well"
#endif

// clang-format off
void (insque)(void *element, void *pred) {
  insque(element, pred);
}
