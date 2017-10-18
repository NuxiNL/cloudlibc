// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <netdb.h>
#include <stdlib.h>
#include <uv.h>

void freeaddrinfo(struct addrinfo *ai) {
  // All entries are stored contiguously and can be freed by calling
  // free() on the first entry.
  free(ai);
}

__strong_reference(freeaddrinfo, uv_freeaddrinfo);
