// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdatomic.h>

#ifndef atomic_signal_fence
#error "atomic_signal_fence is supposed to be a macro as well"
#endif

// clang-format off
void (atomic_signal_fence)(memory_order order) {
  atomic_signal_fence(order);
}
