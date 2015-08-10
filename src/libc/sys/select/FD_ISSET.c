// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/select.h>

#include <stdbool.h>

#ifndef FD_ISSET
#error "FD_ISSET is supposed to be a macro as well"
#endif

// clang-format off
bool (FD_ISSET)(int fd, const fd_set *fd_set) {
  return FD_ISSET(fd, fd_set);
}
