// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/overflow.h>

#include <net/if.h>

unsigned int if_nametoindex(const char *ifname) {
  // As the network interface table is not available, the name of a
  // network interface simply corresponds with the shortest decimal
  // representation of the index.
  if (*ifname < '1' || *ifname > '9')
    return 0;

  unsigned int idx = 0;
  do {
    if (*ifname < '0' || *ifname > '9' || mul_overflow(idx, 10, &idx) ||
        add_overflow(idx, *ifname - '0', &idx))
      return 0;
  } while (*++ifname != '\0');
  return idx;
}
