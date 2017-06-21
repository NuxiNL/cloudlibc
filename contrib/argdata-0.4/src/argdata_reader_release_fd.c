// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "argdata_reader_impl.h"

void argdata_reader_release_fd(argdata_reader_t *ar, int fd) {
  assert(ar->finished &&
         "Attempted to release file descriptor on reader that is in-flight");
  assert(fd >= 0 && "Attempted to release invalid file descriptor");
  for (size_t i = 0; i < ar->fds_len; ++i) {
    if (ar->fds[i] == fd) {
      ar->fds[i] = -1;
      return;
    }
  }
  assert(0 && "Attempted to release an unowned file descriptor");
}
