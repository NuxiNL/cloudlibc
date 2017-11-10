// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/capsicum.h>

int cap_rights_get(int fildes, cap_rights_t *rights) {
  // Simply merge the inheriting rights with the base rights.
  cap_rights_t inheriting;
  int result = cap_rights_get_explicit(fildes, rights, &inheriting);
  if (result != -1)
    cap_rights_merge(rights, &inheriting);
  return result;
}
