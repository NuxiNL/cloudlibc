// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/stat.h>

#include <fcntl.h>
#include <testing.h>
#include <unistd.h>

TEST(dup2, file) {
  int fd1 = openat(fd_tmp, "file1", O_CREAT | O_EXCL | O_WRONLY);
  ASSERT_LE(0, fd1);

  int fd2 = openat(fd_tmp, "file2", O_CREAT | O_EXCL | O_WRONLY);
  ASSERT_LE(0, fd2);
  ASSERT_NE(fd1, fd2);

  // Both descriptors should point to a different file.
  struct stat sb1, sb2;
  ASSERT_EQ(0, fstat(fd1, &sb1));
  ASSERT_EQ(0, fstat(fd2, &sb2));
  ASSERT_EQ(sb1.st_dev, sb2.st_dev);
  ASSERT_NE(sb1.st_ino, sb2.st_ino);

  ASSERT_EQ(fd2, dup2(fd1, fd2));

  // Both descriptors should now point to the same file.
  ASSERT_EQ(0, fstat(fd1, &sb1));
  ASSERT_EQ(0, fstat(fd2, &sb2));
  ASSERT_EQ(sb1.st_dev, sb2.st_dev);
  ASSERT_EQ(sb1.st_ino, sb2.st_ino);

  ASSERT_EQ(0, close(fd1));
  ASSERT_EQ(0, close(fd2));
}
