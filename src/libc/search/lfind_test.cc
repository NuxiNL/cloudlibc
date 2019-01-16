// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <search.h>
#include <stddef.h>
#include <iterator>

#include "gtest/gtest.h"

TEST(lfind, zero) {
  // Empty list should not be accessed.
  size_t nel = 0;
  ASSERT_EQ(NULL, lfind((const void *)1, (const void *)2, &nel, 3,
                        (int (*)(const void *, const void *))4));
  ASSERT_EQ(0, nel);
}

static int compar(const void *a, const void *b) {
  return *(int *)b - *(int *)a;
}

TEST(lfind, example) {
  int v[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  size_t nel = std::size(v);

  for (int i = -nel; i < 2 * (int)nel; ++i) {
    if (i < 0 || i >= (int)nel)
      ASSERT_EQ(NULL, lfind(&i, v, &nel, sizeof(i), compar));
    else
      ASSERT_EQ(&v[i], lfind(&i, v, &nel, sizeof(i), compar));
    ASSERT_EQ(std::size(v), nel);
  }
}
