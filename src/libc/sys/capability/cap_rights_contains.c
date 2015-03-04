// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/capability.h>

#include <stdbool.h>

bool cap_rights_contains(const cap_rights_t *big, const cap_rights_t *little) {
  return (big->__value & little->__value) == little->__value;
}
