// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <fenv.h>

#include "gtest/gtest.h"

// TODO(ed): We should also test non-stop mode. Unfortunately, both
// Clang and GCC don't support FENV_ACCESS.

TEST(feholdexcept, example) {
  ASSERT_EQ(0, feraiseexcept(FE_ALL_EXCEPT));

  // Save existing environment. All exceptions should be cleared.
  fenv_t env;
  ASSERT_EQ(0, feholdexcept(&env));
  ASSERT_EQ(0, fetestexcept(FE_ALL_EXCEPT));

  // Restoring the environment should raise the exceptions again.
  ASSERT_EQ(0, fesetenv(&env));
  ASSERT_EQ(FE_ALL_EXCEPT, fetestexcept(FE_ALL_EXCEPT));
}
