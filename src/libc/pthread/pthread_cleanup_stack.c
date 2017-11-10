// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <pthread.h>
#include <threads.h>

thread_local const struct __pthread_cleanup *__pthread_cleanup_stack = NULL;
