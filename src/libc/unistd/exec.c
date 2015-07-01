// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalls.h>

#include <argdata.h>
#include <unistd.h>

int exec(int fd, const argdata_t *ad) {
  // TODO(ed): Convert argument data to values passed to exec().
  return cloudabi_sys_proc_exec(fd, NULL, 0, NULL, 0);
}
