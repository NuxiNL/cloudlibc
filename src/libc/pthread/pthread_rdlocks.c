// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/pthread.h>

thread_local unsigned int __pthread_rdlocks = 0;
