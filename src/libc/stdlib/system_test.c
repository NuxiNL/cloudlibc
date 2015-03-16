// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <stdlib.h>
#include <testing.h>

TEST(system, example) {
  // No command processor available.
  ASSERT_EQ(0, system(NULL));
  ASSERT_EQ(-1, system("ls -l"));
  ASSERT_EQ(ENOSYS, errno);
}
