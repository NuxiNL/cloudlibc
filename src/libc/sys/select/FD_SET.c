// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/select.h>

#ifndef FD_SET
#error "FD_SET is supposed to be a macro as well"
#endif

// clang-format off
void (FD_SET)(int fd, fd_set *fd_set) {
  FD_SET(fd, fd_set);
}
