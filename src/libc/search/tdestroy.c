// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <search.h>
#include <stdlib.h>

void tdestroy(TNODE_t *root, void (*destructor)(void *)) {
  if (root != NULL) {
    tdestroy(root->__left, destructor);
    destructor(root->key);
    tdestroy(root->__right, destructor);
    free(root);
  }
}
