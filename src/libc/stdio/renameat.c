// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/errno.h>
#include <common/syscalls.h>

#include <errno.h>
#include <stdio.h>
#include <string.h>

int renameat(int oldfd, const char *old, int newfd, const char *new) {
  cloudabi_errno_t error = cloudabi_sys_file_rename(oldfd, old, strlen(old),
                                                    newfd, new, strlen(new));
  if (error != 0) {
    errno = errno_fixup_directory(oldfd, errno_fixup_directory(newfd, error));
    return -1;
  }
  return 0;
}
