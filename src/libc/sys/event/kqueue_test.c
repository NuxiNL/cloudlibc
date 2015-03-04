// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/event.h>
#include <sys/stat.h>

#include <testing.h>
#include <unistd.h>

TEST(kqueue, example) {
  int fd = kqueue();
  ASSERT_LE(0, fd);

  struct stat sb;
  ASSERT_EQ(0, fstat(fd, &sb));
  ASSERT_TRUE(S_TYPEISPOLL(sb.st_mode));

  ASSERT_EQ(0, close(fd));
}
