// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalls.h>

#include <stdatomic.h>
#include <stdlib.h>
#include <stdnoreturn.h>

#include "stdlib_impl.h"

noreturn void quick_exit(int status) {
  // Invoke at_quick_exit() functions.
  for (struct at_quick_exit *entry =
           atomic_load_explicit(&__at_quick_exit_last, memory_order_relaxed);
       entry != NULL; entry = entry->previous) {
    entry->func();
  }

  // Terminate process.
  cloudabi_sys_proc_exit(status);
}
