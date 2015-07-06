// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalls.h>

#include <stdnoreturn.h>

noreturn void __stack_chk_fail(void);

noreturn void __stack_chk_fail(void) {
  cloudabi_sys_proc_raise(CLOUDABI_SIGABRT);
  cloudabi_sys_proc_exit(1);
}
