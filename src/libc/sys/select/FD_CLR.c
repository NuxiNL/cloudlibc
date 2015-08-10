// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/select.h>

#ifndef FD_CLR
#error "FD_CLR is supposed to be a macro as well"
#endif

// clang-format off
void (FD_CLR)(int fd, fd_set *fd_set) {
  FD_CLR(fd, fd_set);
}
