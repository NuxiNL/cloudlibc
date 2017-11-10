// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <signal.h>

#ifndef sigdelset
#error "sigdelset is supposed to be a macro as well"
#endif

// clang-format off
int (sigdelset)(sigset_t *set, int signo) {
  return sigdelset(set, signo);
}
