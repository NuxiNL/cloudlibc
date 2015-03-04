// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <dirent.h>
#include <errno.h>
#include <stddef.h>

#include "dirent_impl.h"

struct dirent *readdir(DIR *dirp) {
  struct dirent *result;
  int error = readdir_r(dirp, &dirp->dirent, &result);
  if (error != 0) {
    errno = error;
    return NULL;
  }
  return result;
}
