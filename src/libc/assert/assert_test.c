// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#undef NDEBUG
#include <assert.h>
#include <testing.h>

TEST(assert, enabled) {
  // Assert should accept any kind of scalar expression.
  assert(1);
  assert(1 && "This works");
  assert((char *)1);

  // Expression should be evaluated.
  int i = 0;
  assert((i = 1));
  ASSERT_EQ(1, i);
}

#define NDEBUG
#include <assert.h>

TEST(assert, disabled) {
  // Any kind of expression should be allowed.
  assert(0);
  assert(0 && "This doesn't work");
  assert(NULL);
  assert(some malformed expression);

  // Expression should not be evaluated.
  int i = 0;
  assert((i = 1));
  ASSERT_EQ(0, i);
}
