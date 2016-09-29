// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/assembly.h>

#ifdef __arm__

ENTRY(__aeabi_memset)
ALTENTRY(__aeabi_memset4)
ALTENTRY(__aeabi_memset8)
  mov r3, r2
  mov r2, r1
  mov r1, r3
  b memset
END(__aeabi_memset)

#endif
