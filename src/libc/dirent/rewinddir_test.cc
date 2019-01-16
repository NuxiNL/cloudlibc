// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "gtest/gtest.h"
#include "src/gtest_with_tmpdir/gtest_with_tmpdir.h"

TEST(rewinddir, example) {
  int fd_tmp = gtest_with_tmpdir::CreateTemporaryDirectory();

  // This should already load the first two entries in the directory.
  DIR *dirp = fdopendir(fd_tmp);

  // Create 1000 files in the meantime.
  for (int i = 0; i < 1000; ++i) {
    char filename[8];
    snprintf(filename, sizeof(filename), "file%03d", i);
    ASSERT_EQ(0, close(openat(fd_tmp, filename, O_RDWR | O_CREAT)));
  }

  // We should only see the original first two files.
  size_t count = 0;
  while (readdir(dirp) != NULL)
    ++count;
  ASSERT_EQ(2, count);

  // Rewind and count the files in the directory again.
  rewinddir(dirp);
  count = 0;
  while (readdir(dirp) != NULL)
    ++count;
  ASSERT_EQ(1002, count);

  // Delete the test files again.
  for (int i = 0; i < 1000; ++i) {
    char filename[8];
    snprintf(filename, sizeof(filename), "file%03d", i);
    ASSERT_EQ(0, unlinkat(fd_tmp, filename, 0));
  }

  // We should have only two files now.
  rewinddir(dirp);
  count = 0;
  while (readdir(dirp) != NULL)
    ++count;
  ASSERT_EQ(2, count);
}
