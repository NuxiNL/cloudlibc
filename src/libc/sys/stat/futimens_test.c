// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/stat.h>

#include <fcntl.h>
#include <testing.h>
#include <unistd.h>

#define TIME_PASSED 1416225136

TEST(futimens, example) {
  // Create a FIFO and a symlink pointing to it.
  int fd = openat(fd_tmp, "file", O_CREAT | O_RDWR);
  ASSERT_LE(0, fd);

  // Set timestamps to known initial values.
  {
    struct timespec times[2] = {{.tv_sec = 123, .tv_nsec = 456},
                                {.tv_sec = 234, .tv_nsec = 567}};
    ASSERT_EQ(0, futimens(fd, times));
  }

  // Validate filetypes and timestamps before continuing.
  struct stat sb;
  ASSERT_EQ(0, fstat(fd, &sb));
  ASSERT_TRUE(S_ISREG(sb.st_mode));
  ASSERT_EQ(123, sb.st_atim.tv_sec);
  ASSERT_EQ(456, sb.st_atim.tv_nsec);
  ASSERT_EQ(234, sb.st_mtim.tv_sec);
  ASSERT_EQ(567, sb.st_mtim.tv_nsec);

  // UTIME_OMIT.
  {
    struct timespec times[2] = {{.tv_sec = 777, .tv_nsec = 888},
                                {.tv_nsec = UTIME_OMIT}};
    ASSERT_EQ(0, futimens(fd, times));
  }

  ASSERT_EQ(0, fstat(fd, &sb));
  ASSERT_EQ(777, sb.st_atim.tv_sec);
  ASSERT_EQ(888, sb.st_atim.tv_nsec);
  ASSERT_EQ(234, sb.st_mtim.tv_sec);
  ASSERT_EQ(567, sb.st_mtim.tv_nsec);

  // UTIME_NOW.
  {
    struct timespec times[2] = {{.tv_nsec = UTIME_OMIT},
                                {.tv_nsec = UTIME_NOW}};
    ASSERT_EQ(0, futimens(fd, times));
  }

  ASSERT_EQ(0, fstat(fd, &sb));
  ASSERT_EQ(777, sb.st_atim.tv_sec);
  ASSERT_EQ(888, sb.st_atim.tv_nsec);
  ASSERT_LE(TIME_PASSED, sb.st_mtime);

  // Null pointer.
  ASSERT_EQ(0, futimens(fd, NULL));

  ASSERT_EQ(0, fstat(fd, &sb));
  ASSERT_LE(TIME_PASSED, sb.st_atime);
  ASSERT_LE(TIME_PASSED, sb.st_mtime);

  ASSERT_EQ(0, close(fd));
}
