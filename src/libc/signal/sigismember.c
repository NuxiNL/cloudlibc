// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <signal.h>

#ifndef sigismember
#error "sigismember is supposed to be a macro as well"
#endif

// clang-format off
int (sigismember)(const sigset_t *set, int signo) {
  return sigismember(set, signo);
}
