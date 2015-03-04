// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <locale.h>
#include <signal.h>

void psignal(int signum, const char *message) {
  psignal_l(signum, message, LC_GLOBAL_LOCALE);
}
