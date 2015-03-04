// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/pthread.h>

#include <stddef.h>

_Atomic(struct pthread_atfork *) __pthread_atfork_last = NULL;
