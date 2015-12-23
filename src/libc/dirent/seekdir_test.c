// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/stat.h>

#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <testing.h>
#include <unistd.h>

TEST(seekdir, example) {
  // Create some example files.
  for (int i = 0; i < 1000; ++i) {
    char filename[8];
    snprintf(filename, sizeof(filename), "file%03d", i);
    ASSERT_EQ(0, close(openat(fd_tmp, filename, O_RDWR | O_CREAT)));
  }

  // seekdir(telldir()) should restore to the right location.
  DIR *dirp = fdopendir(fd_tmp);
  size_t count = 0;
  struct dirent *de;
  while ((de = readdir(dirp)) != NULL) {
    if (de->d_name[0] == '.') {
      ASSERT_TRUE(S_ISDIR(de->d_type));
    } else {
      ASSERT_TRUE(S_ISREG(de->d_type));
    }
    seekdir(dirp, telldir(dirp));
    ++count;
  }
  ASSERT_EQ(1002, count);
}
