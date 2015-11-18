// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalls.h>

#include <stdio.h>

noreturn void abort(void) {
  // Flush error messages.
  fflush(stderr);

  cloudabi_sys_proc_raise(CLOUDABI_SIGABRT);
  cloudabi_sys_proc_exit(1);
}
