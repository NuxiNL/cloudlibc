// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <errno.h>
#include <stdlib.h>

int system(const char *command) {
  if (command == NULL) {
    // Caller wants to know whether a command processor is available.
    // Return zero to indicate that none is available.
    return 0;
  } else {
    // Caller attempted to invoke the command processor. Fail.
    errno = ENOSYS;
    return -1;
  }
}
