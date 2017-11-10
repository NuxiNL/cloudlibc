// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdatomic.h>
#include <stddef.h>

#include "stdlib_impl.h"

_Atomic(struct atexit *) __atexit_last = ATOMIC_VAR_INIT(NULL);
