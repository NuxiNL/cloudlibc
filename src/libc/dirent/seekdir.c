// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <dirent.h>

#include "dirent_impl.h"

void seekdir(DIR *dirp, long loc) {
  // Update cookie.
  dirp->cookie = (unsigned long)loc;
  // Mark entire buffer as processed to force a read of new data.
  dirp->buffer_length = dirp->buffer_processed = sizeof(dirp->buffer);
}
