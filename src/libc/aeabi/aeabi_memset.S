// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

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
