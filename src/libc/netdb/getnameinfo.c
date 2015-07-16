// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <netdb.h>
#include <stddef.h>

int getnameinfo(const struct sockaddr *restrict sa, size_t salen,
                char *restrict node, size_t nodelen, char *restrict service,
                size_t servicelen, int flags) {
  // TODO(ed): Implement.
  return EAI_SYSTEM;
}
