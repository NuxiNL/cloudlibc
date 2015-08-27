// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <net/if.h>

#include <string.h>

char *if_indextoname(unsigned int ifindex, char *ifname) {
  // Interface zero is reserved.
  if (ifindex == 0)
    return NULL;

  // Convert interface index to decimal representation.
  char buf[IF_NAMESIZE];
  char *str = buf + sizeof(buf);
  *--str = '\0';
  do {
    *--str = ifindex % 10 + '0';
    ifindex /= 10;
  } while (ifindex != 0);
  memcpy(ifname, str, buf + sizeof(buf) - str);
  return ifname;
}
