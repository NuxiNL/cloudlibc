// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalldefs.h>

#include <dirent.h>

#include "dirent_impl.h"

void rewinddir(DIR *dirp) {
  // Update cookie.
  dirp->cookie = CLOUDABI_DIRCOOKIE_START;
  // Mark entire buffer as processed to force a read of new data.
  dirp->buffer_length = dirp->buffer_processed = sizeof(dirp->buffer);
}
