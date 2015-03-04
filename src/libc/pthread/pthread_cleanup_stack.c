// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <pthread.h>
#include <threads.h>

thread_local const struct __pthread_cleanup *__pthread_cleanup_stack = NULL;
