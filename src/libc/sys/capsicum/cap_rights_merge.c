// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/capsicum.h>

cap_rights_t *cap_rights_merge(cap_rights_t *dst, const cap_rights_t *src) {
  dst->__value |= src->__value;
  return dst;
}
