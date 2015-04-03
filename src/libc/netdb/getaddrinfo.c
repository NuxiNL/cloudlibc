// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <netdb.h>

int getaddrinfo(const char *restrict nodename, const char *restrict servname,
                const struct addrinfo *restrict hints,
                struct addrinfo **restrict res) {
  // TODO(edje): Implement.
  return EAI_SYSTEM;
}
