// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/capability.h>

#include <errno.h>

int cap_rights_limit(int fildes, const cap_rights_t *rights) {
  // Attempt to decompose the rights back into base and inheriting.
  cap_rights_t base, inheriting;
  if (cap_rights_get_explicit(fildes, &base, &inheriting) == -1)
    return -1;
  if ((rights->__value & ~(base.__value | inheriting.__value)) != 0) {
    // Attempted to extend rights, which is impossible.
    errno = ENOTCAPABLE;
    return -1;
  }
  base.__value &= rights->__value;
  inheriting.__value &= rights->__value;
  return cap_rights_limit_explicit(fildes, &base, &inheriting);
}
