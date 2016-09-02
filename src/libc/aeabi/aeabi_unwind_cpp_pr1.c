// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdlib.h>

#include "aeabi_impl.h"

#ifdef __arm__

__weak_symbol void __aeabi_unwind_cpp_pr1(void) {
  // This function should never be called, as the application should use
  // the version provided by libunwind.
  abort();
}

#endif
