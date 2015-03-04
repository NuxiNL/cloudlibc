// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalls.h>

#include <sys/mman.h>

#include <assert.h>
#include <errno.h>

static_assert(MS_ASYNC == CLOUDABI_MS_ASYNC, "Value mismatch");
static_assert(MS_INVALIDATE == CLOUDABI_MS_INVALIDATE, "Value mismatch");
static_assert(MS_SYNC == CLOUDABI_MS_SYNC, "Value mismatch");

int msync(void *addr, size_t len, int flags) {
  cloudabi_errno_t error = cloudabi_sys_mem_sync(addr, len, flags);
  if (error != 0) {
    errno = error;
    return -1;
  }
  return 0;
}
