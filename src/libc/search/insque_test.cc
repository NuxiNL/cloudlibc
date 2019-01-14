// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <search.h>
#include <stddef.h>

#include "gtest/gtest.h"

struct que {
  struct que *succ;
  struct que *pred;
};

TEST(insque, null) {
  struct que element;
  insque(&element, NULL);
  ASSERT_EQ(NULL, element.succ);
  ASSERT_EQ(NULL, element.pred);
}

TEST(insque, tail) {
  struct que element;
  struct que pred = {.succ = NULL, .pred = (struct que *)42};
  insque(&element, &pred);
  ASSERT_EQ(NULL, element.succ);
  ASSERT_EQ(&pred, element.pred);
  ASSERT_EQ(&element, pred.succ);
  ASSERT_EQ((struct que *)42, pred.pred);
}

TEST(insque, middle) {
  struct que element;
  struct que succ = {.succ = (struct que *)13, .pred = (struct que *)37};
  struct que pred = {.succ = &succ, .pred = (struct que *)42};
  insque(&element, &pred);
  ASSERT_EQ(&succ, element.succ);
  ASSERT_EQ(&pred, element.pred);
  ASSERT_EQ((struct que *)13, succ.succ);
  ASSERT_EQ(&element, succ.pred);
  ASSERT_EQ(&element, pred.succ);
  ASSERT_EQ((struct que *)42, pred.pred);
}
