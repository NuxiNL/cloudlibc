// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdlib.h>

#include "aeabi_impl.h"

#ifdef __arm__

__weak_symbol void __aeabi_unwind_cpp_pr0(void) {
  // This function should never be called, as the application should use
  // the version provided by libunwind.
  abort();
}

#endif
