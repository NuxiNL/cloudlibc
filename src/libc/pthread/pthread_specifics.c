// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/pthread.h>

thread_local struct pthread_specifics __pthread_specifics =
    LIST_HEAD_INITIALIZER(__pthread_specifics);
