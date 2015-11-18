// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/pthread.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

noreturn void __pthread_terminate(cloudabi_errno_t error, const char *message) {
  errno = error;
  perror(message);
  abort();
}
