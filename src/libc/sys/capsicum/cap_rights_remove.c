// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/capsicum.h>

cap_rights_t *cap_rights_remove(cap_rights_t *dst, const cap_rights_t *src) {
  dst->__value &= ~src->__value;
  return dst;
}
