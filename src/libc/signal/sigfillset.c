// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <signal.h>

#ifndef sigfillset
#error "sigfillset is supposed to be a macro as well"
#endif

// clang-format off
int (sigfillset)(sigset_t *set) {
  return sigfillset(set);
}
