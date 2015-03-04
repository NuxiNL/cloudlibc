// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalls.h>

#include <assert.h>
#include <errno.h>
#include <fcntl.h>

static_assert(POSIX_FADV_DONTNEED == CLOUDABI_ADVICE_DONTNEED,
              "Value mismatch");
static_assert(POSIX_FADV_NOREUSE == CLOUDABI_ADVICE_NOREUSE, "Value mismatch");
static_assert(POSIX_FADV_NORMAL == CLOUDABI_ADVICE_NORMAL, "Value mismatch");
static_assert(POSIX_FADV_RANDOM == CLOUDABI_ADVICE_RANDOM, "Value mismatch");
static_assert(POSIX_FADV_SEQUENTIAL == CLOUDABI_ADVICE_SEQUENTIAL,
              "Value mismatch");
static_assert(POSIX_FADV_WILLNEED == CLOUDABI_ADVICE_WILLNEED,
              "Value mismatch");

int posix_fadvise(int fd, off_t offset, off_t len, int advice) {
  if (offset < 0 || len < 0)
    return EINVAL;
  return cloudabi_sys_file_advise(fd, offset, len, advice);
}
