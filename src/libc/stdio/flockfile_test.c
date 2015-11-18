// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdio.h>
#include <testing.h>

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
