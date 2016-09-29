// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/assembly.h>

#ifdef __arm__

ENTRY(__aeabi_memclr)
ALTENTRY(__aeabi_memclr4)
ALTENTRY(__aeabi_memclr8)
  mov r2, r1
  mov r1, #0
  b memset
END(__aeabi_memclr)

#endif
