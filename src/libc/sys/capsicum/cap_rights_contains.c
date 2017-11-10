// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/capsicum.h>

#include <stdbool.h>

bool cap_rights_contains(const cap_rights_t *big, const cap_rights_t *little) {
  // The first set should be a superset of the second.
  return (big->__value & little->__value) == little->__value;
}
