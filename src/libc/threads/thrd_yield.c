// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sched.h>
#include <threads.h>

void thrd_yield(void) {
  sched_yield();
}
