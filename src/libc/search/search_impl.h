// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#ifndef SEARCH_SEARCH_IMPL_H
#define SEARCH_SEARCH_IMPL_H

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>

// Bookkeeping for storing a path in a balanced binary search tree from
// the root to a leaf node.
//
// For an AVL tree we know that its maximum height of a tree is bounded
// by approximately 1.44 * log2(n) - 0.328. Given that the number of
// entries of the tree is constrained by the size of the address space,
// two uintptr_t's provide sufficient space to store the path from the
// root to any leaf.
struct path {
  uintptr_t steps[2];
  unsigned int nsteps;
};

// Initializes the path structure with a zero-length path.
static inline void path_init(struct path *p) {
  p->steps[0] = 0;
  p->steps[1] = 0;
  p->nsteps = 0;
}

#define STEPS_BIT (sizeof(uintptr_t) * CHAR_BIT)

// Pushes a step to the left to the end of the path.
static inline void path_taking_left(struct path *p) {
  p->steps[p->nsteps / STEPS_BIT] |= (uintptr_t)1 << (p->nsteps % STEPS_BIT);
  ++p->nsteps;
}

// Pushes a step to the right to the end of the path.
static inline void path_taking_right(struct path *p) {
  ++p->nsteps;
}

// Pops the first step from the path and returns whether it was a step
// to the left.
static inline bool path_took_left(struct path *p) {
  bool result = p->steps[0] & 0x1;
  p->steps[0] = (p->steps[0] >> 1) | (p->steps[1] << (STEPS_BIT - 1));
  p->steps[1] >>= 1;
  return result;
}

#undef STEPS_BIT

#endif
