// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/crt.h>
#include <common/syscalls.h>

#include <stdio.h>
#include <stdlib.h>

noreturn void exit(int ret) {
  // Flush error messages.
  fflush(stderr);

  // Invoke global destructors.
  void (**dtor)(void) = __dtors_start;
  while (dtor < __dtors_stop)
    (*dtor++)();

  // Terminate process.
  cloudabi_sys_proc_exit(ret);
}
