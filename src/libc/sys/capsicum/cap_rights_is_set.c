// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/capsicum.h>

#include <stdarg.h>
#include <stdbool.h>

bool __cap_rights_is_set(const cap_rights_t *rights, ...) {
  // Construct bitmask from arguments.
  __cap_rights_bits_t bits = 0;
  va_list ap;
  va_start(ap, rights);
  for (;;) {
    __cap_rights_bits_t right = va_arg(ap, __cap_rights_bits_t);
    if (right == _CAP_SENTINEL)
      break;
    bits |= right;
  }
  va_end(ap);

  // Validate that all bits have been set.
  return (rights->__value & bits) == bits;
}
