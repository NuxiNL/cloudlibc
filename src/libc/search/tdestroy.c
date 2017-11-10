// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <search.h>
#include <stdlib.h>

void tdestroy(posix_tnode *root, void (*destructor)(void *)) {
  if (root != NULL) {
    tdestroy(root->__left, destructor);
    destructor(root->key);
    tdestroy(root->__right, destructor);
    free(root);
  }
}
