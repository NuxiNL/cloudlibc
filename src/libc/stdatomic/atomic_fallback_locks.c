// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <assert.h>
#include <pthread.h>

#include "stdatomic_impl.h"

pthread_rwlock_t __atomic_fallback_locks[] = {
    PTHREAD_RWLOCK_INITIALIZER, PTHREAD_RWLOCK_INITIALIZER,
    PTHREAD_RWLOCK_INITIALIZER, PTHREAD_RWLOCK_INITIALIZER,
    PTHREAD_RWLOCK_INITIALIZER, PTHREAD_RWLOCK_INITIALIZER,
    PTHREAD_RWLOCK_INITIALIZER, PTHREAD_RWLOCK_INITIALIZER,
    PTHREAD_RWLOCK_INITIALIZER, PTHREAD_RWLOCK_INITIALIZER,
    PTHREAD_RWLOCK_INITIALIZER, PTHREAD_RWLOCK_INITIALIZER,
    PTHREAD_RWLOCK_INITIALIZER, PTHREAD_RWLOCK_INITIALIZER,
    PTHREAD_RWLOCK_INITIALIZER, PTHREAD_RWLOCK_INITIALIZER,
};
static_assert(__arraycount(__atomic_fallback_locks) ==
                  1 << ATOMIC_FALLBACK_NBITS,
              "Array does not contain the right number of locks");
