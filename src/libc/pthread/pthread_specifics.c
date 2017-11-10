// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/pthread.h>

thread_local struct pthread_specifics __pthread_specifics =
    LIST_HEAD_INITIALIZER(__pthread_specifics);
