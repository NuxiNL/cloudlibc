// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <search.h>
#include <stdlib.h>

void tdestroy(void *root, void (*destructor)(void *)) {
  if (root != NULL) {
    struct __tnode *n = root;
    tdestroy(n->__left, destructor);
    destructor(n->__key);
    tdestroy(n->__right, destructor);
    free(n);
  }
}
