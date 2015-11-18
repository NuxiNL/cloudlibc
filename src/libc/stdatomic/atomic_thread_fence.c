// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdatomic.h>

#ifndef atomic_thread_fence
#error "atomic_thread_fence is supposed to be a macro as well"
#endif

// clang-format off
void (atomic_thread_fence)(memory_order order) {
  atomic_thread_fence(order);
}
