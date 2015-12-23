// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <signal.h>

#ifndef sigismember
#error "sigismember is supposed to be a macro as well"
#endif

// clang-format off
int (sigismember)(const sigset_t *set, int signo) {
  return sigismember(set, signo);
}
