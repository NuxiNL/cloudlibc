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

TEST(remque, singleton) {
  struct que element = {.succ = NULL, .pred = NULL};
  remque(&element);
}

TEST(remque, head) {
  struct que succ = {.succ = (struct que *)13, .pred = (struct que *)37};
  struct que element = {.succ = &succ, .pred = NULL};
  remque(&element);
  ASSERT_EQ((struct que *)13, succ.succ);
  ASSERT_EQ(NULL, succ.pred);
}

TEST(remque, tail) {
  struct que pred = {.succ = (struct que *)13, .pred = (struct que *)37};
  struct que element = {.succ = NULL, .pred = &pred};
  remque(&element);
  ASSERT_EQ(NULL, pred.succ);
  ASSERT_EQ((struct que *)37, pred.pred);
}

TEST(remque, middle) {
  struct que succ = {.succ = (struct que *)19, .pred = (struct que *)86};
  struct que pred = {.succ = (struct que *)13, .pred = (struct que *)37};
  struct que element = {.succ = &succ, .pred = &pred};
  remque(&element);
  ASSERT_EQ((struct que *)19, succ.succ);
  ASSERT_EQ(&pred, succ.pred);
  ASSERT_EQ(&succ, pred.succ);
  ASSERT_EQ((struct que *)37, pred.pred);
}
