// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/program_getuuid.h>

#include <pthread.h>

pthread_once_t __program_getuuid_init = PTHREAD_ONCE_INIT;
