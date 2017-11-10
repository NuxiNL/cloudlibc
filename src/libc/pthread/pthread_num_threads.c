// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/pthread.h>
#include <common/refcount.h>

refcount_t __pthread_num_threads = REFCOUNT_INIT(1);
