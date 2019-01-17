// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdio.h>

#include "gtest/gtest.h"

TEST(flockfile, example) __no_lock_analysis {
  FILE *fp = tmpfile();
  ASSERT_NE(NULL, fp);

  // Acquire and release the lock recursively.
  flockfile(fp);
  flockfile(fp);
  flockfile(fp);
  funlockfile(fp);
  funlockfile(fp);
  funlockfile(fp);

  ASSERT_EQ(0, fclose(fp));
}
