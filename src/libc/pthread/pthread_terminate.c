// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/pthread.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

noreturn void __pthread_terminate(cloudabi_errno_t error, const char *message) {
  errno = error;
  perror(message);
  abort();
}
