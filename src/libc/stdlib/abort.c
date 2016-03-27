// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <cloudabi_syscalls.h>
#include <stdio.h>
#include <stdnoreturn.h>

noreturn void abort(void) {
  // Flush error messages.
  fflush(stderr);

  cloudabi_sys_proc_raise(CLOUDABI_SIGABRT);
  cloudabi_sys_proc_exit(1);
}
