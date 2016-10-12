// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <search.h>

#ifndef twalk
#error "twalk is supposed to be a macro as well"
#endif

// clang-format off
void (twalk)(const posix_tnode *root,
             void (*action)(const posix_tnode *, VISIT, int)) {
  twalk(root, action);
}
