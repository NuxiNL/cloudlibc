// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/pthread.h>

#include <pthread.h>

pthread_mutex_t __pthread_key_freelist_lock = PTHREAD_MUTEX_INITIALIZER;
struct pthread_key_freelist __pthread_key_freelist =
    SLIST_HEAD_INITIALIZER(__pthread_key_freelist);
