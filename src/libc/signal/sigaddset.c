// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <signal.h>

#ifndef sigaddset
#error "sigaddset is supposed to be a macro as well"
#endif

// clang-format off
int (sigaddset)(sigset_t *set, int signo) {
  return sigaddset(set, signo);
}
