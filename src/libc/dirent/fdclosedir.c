// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <dirent.h>
#include <stdlib.h>
#include <pthread.h>

#include "dirent_impl.h"

int fdclosedir(DIR *dirp) {
  int fd = dirp->fd;
  pthread_mutex_destroy(&dirp->lock);
  free(dirp);
  return fd;
}
