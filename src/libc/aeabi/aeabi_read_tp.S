// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/assembly.h>

#ifdef __arm__

ENTRY(__aeabi_read_tp)
  // Return the TLS base address.
  mrc p15, 0, r0, cr13, cr0, 2
  bx lr
END(__aeabi_read_tp)

#endif
