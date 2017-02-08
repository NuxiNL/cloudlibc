// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <execinfo.h>
#include <stdio.h>

#include "execinfo_impl.h"

void backtrace_symbols_fd(void *const *buffer, size_t size, int fd) {
  for (size_t i = 0; i < size; ++i) {
    struct frame f;
    frame_get(&f, buffer[i]);
    FRAME_PRINT(&f, dprintf, "\n", fd);
  }
}
