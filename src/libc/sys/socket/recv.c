// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/socket.h>

#ifndef recv
#error "recv is supposed to be a macro as well"
#endif

// clang-format off
ssize_t (recv)(int socket, void *buffer, size_t length, int flags) {
  return recv(socket, buffer, length, flags);
}
