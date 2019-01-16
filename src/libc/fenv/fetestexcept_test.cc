// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <fenv.h>

#include "gtest/gtest.h"

// TODO(ed): In addition to using feraiseexcept(), we should actually
// trigger the exceptions. Unfortunately, both Clang and GCC don't
// support FENV_ACCESS.

TEST(fetestexcept, example) {
  // Clear all exceptions.
  ASSERT_EQ(0, feclearexcept(FE_ALL_EXCEPT));
  ASSERT_EQ(0, fetestexcept(FE_ALL_EXCEPT));

  // Set all the exceptions one by one.
  ASSERT_EQ(0, feraiseexcept(FE_DIVBYZERO));
  ASSERT_EQ(0, fetestexcept(0));
  ASSERT_EQ(FE_DIVBYZERO, fetestexcept(FE_DIVBYZERO));
  ASSERT_EQ(FE_DIVBYZERO, fetestexcept(FE_ALL_EXCEPT));

  ASSERT_EQ(0, feraiseexcept(FE_INEXACT));
  ASSERT_EQ(0, fetestexcept(0));
  ASSERT_EQ(FE_DIVBYZERO, fetestexcept(FE_DIVBYZERO));
  ASSERT_EQ(FE_INEXACT, fetestexcept(FE_INEXACT));
  ASSERT_EQ(FE_DIVBYZERO | FE_INEXACT, fetestexcept(FE_ALL_EXCEPT));

  ASSERT_EQ(0, feraiseexcept(FE_INVALID));
  ASSERT_EQ(FE_DIVBYZERO | FE_INEXACT | FE_INVALID,
            fetestexcept(FE_ALL_EXCEPT));

  // Save two exception flags; one set, one not set.
  fexcept_t excepts;
  ASSERT_EQ(0, fegetexceptflag(&excepts, FE_OVERFLOW | FE_INVALID));

  ASSERT_EQ(0, feraiseexcept(FE_OVERFLOW));
  ASSERT_EQ(FE_DIVBYZERO | FE_INEXACT | FE_INVALID | FE_OVERFLOW,
            fetestexcept(FE_ALL_EXCEPT));

  ASSERT_EQ(0, feraiseexcept(FE_UNDERFLOW));
  ASSERT_EQ(FE_DIVBYZERO | FE_INEXACT | FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW,
            fetestexcept(FE_ALL_EXCEPT));

  // Clear all exceptions once more.
  ASSERT_EQ(0, feclearexcept(FE_ALL_EXCEPT));
  ASSERT_EQ(0, fetestexcept(FE_ALL_EXCEPT));

  // Set all of them using FE_ALL_EXCEPT.
  ASSERT_EQ(0, feraiseexcept(FE_ALL_EXCEPT));
  ASSERT_EQ(FE_ALL_EXCEPT, fetestexcept(FE_ALL_EXCEPT));

  // Clear exceptions by restoring the default environment.
  fenv_t backup;
  ASSERT_EQ(0, fegetenv(&backup));
  ASSERT_EQ(0, fesetenv(FE_DFL_ENV));
  ASSERT_EQ(0, fetestexcept(FE_ALL_EXCEPT));

  // Restore backed up environment.
  ASSERT_EQ(0, fesetenv(&backup));
  ASSERT_EQ(FE_ALL_EXCEPT, fetestexcept(FE_ALL_EXCEPT));

  // Clear the saved exception flags. FE_OVERFLOW should be cleared.
  ASSERT_EQ(0, fesetexceptflag(&excepts, FE_OVERFLOW | FE_INVALID));
  ASSERT_EQ(FE_ALL_EXCEPT & ~FE_OVERFLOW, fetestexcept(FE_ALL_EXCEPT));
}
