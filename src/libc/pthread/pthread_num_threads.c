// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/pthread.h>
#include <common/refcount.h>

refcount_t __pthread_num_threads = REFCOUNT_INIT(1);
