// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/arc4random.h>

#include <stddef.h>
#include <threads.h>

thread_local size_t __arc4random_bytesleft = 0;
