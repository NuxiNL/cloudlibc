// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/stat.h>

#include <errno.h>
#include <testing.h>
#include <unistd.h>

TEST(pipe, readwrite) {
  // Create pipe.
  int fds[2];
  ASSERT_EQ(0, pipe(fds));

  // Validate stat structures.
  struct stat sb;
  ASSERT_EQ(0, fstat(fds[0], &sb));
  ASSERT_TRUE(S_ISFIFO(sb.st_mode));
  ASSERT_EQ(0, fstat(fds[1], &sb));
  ASSERT_TRUE(S_ISFIFO(sb.st_mode));

  // Write data into it.
  ASSERT_EQ(5, write(fds[1], "Hello", 5));

  // Extract data again.
  char buf[6];
  ASSERT_EQ(5, read(fds[0], buf, sizeof(buf)));
  ASSERT_ARREQ("Hello", buf, 5);

  // Close read end. Write should return EPIPE, but a signal should not
  // be delivered.
  ASSERT_EQ(0, close(fds[0]));
  char f = '!';
  ASSERT_EQ(-1, write(fds[1], &f, 1));
  ASSERT_EQ(EPIPE, errno);

  ASSERT_EQ(0, close(fds[1]));
}
