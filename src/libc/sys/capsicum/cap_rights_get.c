// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/capsicum.h>

int cap_rights_get(int fildes, cap_rights_t *rights) {
  // Simply merge the inheriting rights with the base rights.
  cap_rights_t inheriting;
  int result = cap_rights_get_explicit(fildes, rights, &inheriting);
  if (result != -1)
    cap_rights_merge(rights, &inheriting);
  return result;
}
