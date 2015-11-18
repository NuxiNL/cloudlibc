// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/pthread.h>

#include <stdatomic.h>
#include <stddef.h>

_Atomic(struct pthread_atfork *) __pthread_atfork_last = ATOMIC_VAR_INIT(NULL);
