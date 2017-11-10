// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

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
