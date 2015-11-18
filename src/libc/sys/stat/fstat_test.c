// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/stat.h>

#include <fcntl.h>
#include <testing.h>
#include <unistd.h>

TEST(fstat, example) {
  // Create a file for testing.
  int fd = openat(fd_tmp, "file", O_CREAT | O_RDWR);
  ASSERT_LE(0, fd);
  ASSERT_EQ(5, write(fd, "Hello", 5));

  // Test properties.
  struct stat sb1;
  ASSERT_EQ(0, fstat(fd, &sb1));
  ASSERT_TRUE(S_ISREG(sb1.st_mode));
  ASSERT_EQ(5, sb1.st_size);
  ASSERT_EQ(1, sb1.st_nlink);

  // Compare against results of fstatat().
  struct stat sb2;
  ASSERT_EQ(0, fstatat(fd_tmp, "file", &sb2, 0));
  ASSERT_EQ(sb1.st_dev, sb2.st_dev);
  ASSERT_EQ(sb1.st_ino, sb2.st_ino);
  ASSERT_EQ(sb1.st_mode, sb2.st_mode);
  ASSERT_EQ(sb1.st_nlink, sb2.st_nlink);
  ASSERT_EQ(sb1.st_size, sb2.st_size);
  ASSERT_EQ(sb1.st_atim.tv_sec, sb2.st_atim.tv_sec);
  ASSERT_EQ(sb1.st_atim.tv_nsec, sb2.st_atim.tv_nsec);
  ASSERT_EQ(sb1.st_mtim.tv_sec, sb2.st_mtim.tv_sec);
  ASSERT_EQ(sb1.st_mtim.tv_nsec, sb2.st_mtim.tv_nsec);
  ASSERT_EQ(sb1.st_ctim.tv_sec, sb2.st_ctim.tv_sec);
  ASSERT_EQ(sb1.st_ctim.tv_nsec, sb2.st_ctim.tv_nsec);

  ASSERT_EQ(0, close(fd));
}
