// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/pthread.h>

#include <stdatomic.h>
#include <stddef.h>

_Atomic(struct thread_atexit *) __thread_atexit_last = ATOMIC_VAR_INIT(NULL);
