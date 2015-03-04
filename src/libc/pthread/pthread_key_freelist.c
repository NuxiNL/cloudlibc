// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/pthread.h>

#include <pthread.h>

pthread_mutex_t __pthread_key_freelist_lock = PTHREAD_MUTEX_INITIALIZER;

// During startup, malloc() creates a small number of pthread keys for
// its internal data structures. If no keys are available in the
// freelist, pthread_key_create() will call into malloc() to allocate
// new copies.
//
// To prevent recursion, initialize the freelist with a small number of
// preallocated keys. This allows malloc() to initialize safely.

static struct __pthread_key static_objects[8] = {
    {.freelist = SLIST_ENTRY_INITIALIZER(&static_objects[1])},
    {.freelist = SLIST_ENTRY_INITIALIZER(&static_objects[2])},
    {.freelist = SLIST_ENTRY_INITIALIZER(&static_objects[3])},
    {.freelist = SLIST_ENTRY_INITIALIZER(&static_objects[4])},
    {.freelist = SLIST_ENTRY_INITIALIZER(&static_objects[5])},
    {.freelist = SLIST_ENTRY_INITIALIZER(&static_objects[6])},
    {.freelist = SLIST_ENTRY_INITIALIZER(&static_objects[7])},
};

struct pthread_key_freelist __pthread_key_freelist =
    SLIST_HEAD_INITIALIZER_NONEMPTY(&static_objects[0]);
