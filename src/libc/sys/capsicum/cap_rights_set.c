// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/capsicum.h>

#include <stdarg.h>

cap_rights_t *__cap_rights_set(cap_rights_t *rights, ...) {
  va_list ap;
  va_start(ap, rights);
  for (;;) {
    __cap_rights_bits_t right = va_arg(ap, __cap_rights_bits_t);
    if (right == _CAP_SENTINEL)
      break;
    rights->__value |= right;
  }
  va_end(ap);
  return rights;
}
