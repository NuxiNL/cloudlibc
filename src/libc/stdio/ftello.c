// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/stdio.h>

#include <assert.h>
#include <stdio.h>

off_t ftello(FILE *stream) {
  flockfile(stream);
  if (!fseekable(stream)) {
    // File descriptor is not seekable.
    funlockfile(stream);
    errno = ESPIPE;
    return -1;
  }
  off_t result = ftello_logical(stream);
  funlockfile(stream);
  return result;
}
