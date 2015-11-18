// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdatomic.h>
#include <stddef.h>

#include "stdlib_impl.h"

_Atomic(struct atexit *) __atexit_last = ATOMIC_VAR_INIT(NULL);
