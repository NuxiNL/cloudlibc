// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/capsicum.h>

_Bool cap_rights_equals(const cap_rights_t *rights1,
                        const cap_rights_t *rights2) {
  return rights1->__value == rights2->__value;
}
