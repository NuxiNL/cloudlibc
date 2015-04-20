// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdatomic.h>
#include <stddef.h>

#include "stdlib_impl.h"

_Atomic(struct at_quick_exit *) __at_quick_exit_last = ATOMIC_VAR_INIT(NULL);
