// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <search.h>
#include <stdlib.h>

void *tdelete(const void *restrict key, void **restrict rootp,
              int (*compar)(const void *, const void *)) {
  // Search the tree for a node to delete.
  struct __tnode **root = (struct __tnode **)rootp;
  void *parent_key = (void *)1;
  while (*root != NULL) {
    struct __tnode *node = *root;
    int cmp = compar(key, node->__key);
    if (cmp < 0) {
      root = &node->__left;
      parent_key = &node->__key;
    } else if (cmp > 0) {
      root = &node->__right;
      parent_key = &node->__key;
    } else if (node->__left == NULL) {
      // Node has no left children. This makes deletion easy: simply
      // substitute the node by its right child (if any).
      *root = node->__right;
      free(node);
      return parent_key;
    } else {
      // Node has left children. Find the rightmost leaf of the left child.
      struct __tnode **leaf = &node->__left;
      while ((*leaf)->__right != NULL)
        leaf = &(*leaf)->__right;

      // Replace the node that has to be deleted by this leaf.
      *root = *leaf;
      *leaf = (*leaf)->__left;
      (*root)->__left = node->__left;
      (*root)->__right = node->__right;
      free(node);
      return parent_key;
    }
  }
  return NULL;
}
