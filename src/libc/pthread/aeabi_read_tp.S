// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/assembly.h>

#ifdef __arm__

ENTRY(__aeabi_read_tp)
  // Return the TLS base address.
  mrc p15, 0, r0, cr13, cr0, 2
  bx lr
END(__aeabi_read_tp)

#endif
