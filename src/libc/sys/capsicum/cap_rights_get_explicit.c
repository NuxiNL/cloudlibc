// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalls.h>

#include <sys/capsicum.h>

#include <assert.h>
#include <errno.h>

static_assert(CAP_FEXECVE == CLOUDABI_RIGHT_FEXECVE, "Value mismatch");
static_assert(CAP_FPATHCONF == CLOUDABI_RIGHT_FPATHCONF, "Value mismatch");
static_assert(CAP_FSTAT == CLOUDABI_RIGHT_FSTAT, "Value mismatch");
static_assert(CAP_FSYNC == CLOUDABI_RIGHT_FSYNC, "Value mismatch");
static_assert(CAP_FTRUNCATE == CLOUDABI_RIGHT_FTRUNCATE, "Value mismatch");
static_assert(CAP_FUTIMES == CLOUDABI_RIGHT_FUTIMENS, "Value mismatch");
static_assert(CAP_LINKAT == CLOUDABI_RIGHT_LINK, "Value mismatch");
static_assert(CAP_MKDIRAT == CLOUDABI_RIGHT_MKDIR, "Value mismatch");
static_assert(CAP_MKFIFOAT == CLOUDABI_RIGHT_MKFIFO, "Value mismatch");
static_assert(CAP_MMAP == CLOUDABI_RIGHT_MMAP, "Value mismatch");
static_assert(CAP_READ == CLOUDABI_RIGHT_READ, "Value mismatch");
static_assert(CAP_READDIR == CLOUDABI_RIGHT_READDIR, "Value mismatch");
static_assert(CAP_RENAMEAT == CLOUDABI_RIGHT_RENAME, "Value mismatch");
static_assert(CAP_SEEK == CLOUDABI_RIGHT_SEEK, "Value mismatch");
static_assert(CAP_SEEK_TELL == CLOUDABI_RIGHT_SEEK_TELL, "Value mismatch");
static_assert(CAP_SYMLINKAT == CLOUDABI_RIGHT_SYMLINK, "Value mismatch");
static_assert(CAP_UNLINKAT == CLOUDABI_RIGHT_UNLINK, "Value mismatch");
static_assert(CAP_WRITE == CLOUDABI_RIGHT_WRITE, "Value mismatch");

int cap_rights_get_explicit(int fildes, cap_rights_t *base,
                            cap_rights_t *inheriting) {
  cloudabi_fdstat_t fsb;
  cloudabi_errno_t error = cloudabi_sys_fd_stat_get(fildes, &fsb);
  if (error != 0) {
    errno = error;
    return -1;
  }
  base->__value = fsb.fs_rights_base;
  inheriting->__value = fsb.fs_rights_inheriting;
  return 0;
}
