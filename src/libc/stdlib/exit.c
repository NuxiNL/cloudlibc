// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/crt.h>
#include <common/pthread.h>

#include <cloudabi_syscalls.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>

#include "stdlib_impl.h"

noreturn void exit(int ret) {
  // Invoke cleanup routines registered by __cxa_thread_atexit().
  for (struct thread_atexit *entry =
           atomic_load_explicit(&__thread_atexit_last, memory_order_relaxed);
       entry != NULL; entry = entry->previous)
    entry->func(entry->arg);

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
