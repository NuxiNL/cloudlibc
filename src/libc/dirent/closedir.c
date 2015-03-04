// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <dirent.h>
#include <unistd.h>

int closedir(DIR *dirp) {
  return close(fdclosedir(dirp));
}
