// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/pthread.h>

#include <stdatomic.h>
#include <stddef.h>

_Atomic(struct thread_atexit *) __thread_atexit_last = ATOMIC_VAR_INIT(NULL);
