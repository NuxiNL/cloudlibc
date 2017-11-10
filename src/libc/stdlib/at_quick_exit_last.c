// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdatomic.h>
#include <stddef.h>

#include "stdlib_impl.h"

_Atomic(struct at_quick_exit *) __at_quick_exit_last = ATOMIC_VAR_INIT(NULL);
