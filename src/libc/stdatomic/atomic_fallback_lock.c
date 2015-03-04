// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <pthread.h>

#include "stdatomic_impl.h"

pthread_rwlock_t __atomic_fallback_lock = PTHREAD_RWLOCK_INITIALIZER;
