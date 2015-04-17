// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/crt.h>
#include <common/syscalls.h>

#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>

#include "stdlib_impl.h"

noreturn void exit(int ret) {
  // Invoke global destructors.
  void (**dtor)(void) = __dtors_start;
  while (dtor < __dtors_stop)
    (*dtor++)();

  // Invoke atexit() functions.
  for (struct atexit *entry =
           atomic_load_explicit(&__atexit_last, memory_order_relaxed);
       entry != NULL; entry = entry->previous) {
    if (entry->atexit != NULL)
      entry->atexit();
    if (entry->cxa_atexit != NULL)
      entry->cxa_atexit(entry->cxa_atexit_arg);
  }

  // Flush error messages.
  fflush(stderr);

  // Terminate process.
  cloudabi_sys_proc_exit(ret);
}
