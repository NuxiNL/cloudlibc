// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalls.h>

#include <stdlib.h>
#include <stdnoreturn.h>
#include <unistd.h>

noreturn void _Exit(int status) {
  cloudabi_sys_proc_exit(status);
}

__strong_reference(_Exit, _exit);
