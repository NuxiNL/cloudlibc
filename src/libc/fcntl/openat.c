// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalls.h>

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

static_assert(O_APPEND == CLOUDABI_FDFLAG_APPEND, "Value mismatch");
static_assert(O_DSYNC == CLOUDABI_FDFLAG_DSYNC, "Value mismatch");
static_assert(O_NONBLOCK == CLOUDABI_FDFLAG_NONBLOCK, "Value mismatch");
static_assert(O_RSYNC == CLOUDABI_FDFLAG_RSYNC, "Value mismatch");
static_assert(O_SYNC == CLOUDABI_FDFLAG_SYNC, "Value mismatch");

static_assert(O_CREAT >> 12 == CLOUDABI_O_CREAT, "Value mismatch");
static_assert(O_DIRECTORY >> 12 == CLOUDABI_O_DIRECTORY, "Value mismatch");
static_assert(O_EXCL >> 12 == CLOUDABI_O_EXCL, "Value mismatch");
static_assert(O_TRUNC >> 12 == CLOUDABI_O_TRUNC, "Value mismatch");

int openat(int fd, const char *path, int oflag, ...) {
  // Compute rights corresponding with the access modes provided.
  cloudabi_rights_t min = 0;
  cloudabi_rights_t max =
      CLOUDABI_RIGHT_FEXECVE | CLOUDABI_RIGHT_FSTAT | CLOUDABI_RIGHT_FPATHCONF |
      CLOUDABI_RIGHT_LINK | CLOUDABI_RIGHT_LOOKUP | CLOUDABI_RIGHT_MKDIR |
      CLOUDABI_RIGHT_RENAME | CLOUDABI_RIGHT_SYMLINK | CLOUDABI_RIGHT_UNLINK;
  switch (oflag & O_ACCMODE) {
    case O_RDONLY:
    case O_RDWR:
    case O_WRONLY:
      max |= CLOUDABI_RIGHT_MMAP | CLOUDABI_RIGHT_SEEK;
      if ((oflag & O_RDONLY) != 0) {
        min |= (oflag & O_DIRECTORY) == 0 ? CLOUDABI_RIGHT_READ
                                          : CLOUDABI_RIGHT_READDIR;
        max |= CLOUDABI_RIGHT_READ | CLOUDABI_RIGHT_READDIR;
      }
      if ((oflag & O_WRONLY) != 0) {
        min |= CLOUDABI_RIGHT_WRITE;
        if ((oflag & O_APPEND) == 0)
          min |= CLOUDABI_RIGHT_SEEK;
        max |= CLOUDABI_RIGHT_FSYNC | CLOUDABI_RIGHT_FTRUNCATE |
               CLOUDABI_RIGHT_FUTIMENS | CLOUDABI_RIGHT_WRITE;
      }
      break;
    case O_EXEC:
      min |= CLOUDABI_RIGHT_FEXECVE;
      break;
    case O_SEARCH:
      min |= CLOUDABI_RIGHT_LOOKUP;
      break;
    default:
      errno = EINVAL;
      return -1;
  }
  assert((min & max) == min &&
         "Minimal rights should be a subset of the maximum");

  // Ensure that we can actually obtain the minimal rights needed.
  cloudabi_fdstat_t fsb_cur;
  cloudabi_errno_t error = cloudabi_sys_fd_stat_get(fd, &fsb_cur);
  if (error != 0) {
    errno = error;
    return -1;
  }
  if ((min & fsb_cur.fs_rights_inheriting) != min) {
    errno = ENOTCAPABLE;
    return -1;
  }

  // Path lookup properties.
  cloudabi_lookup_t lookup = (cloudabi_fd_t)fd;
  if ((oflag & O_NOFOLLOW) == 0)
    lookup |= CLOUDABI_LOOKUP_SYMLINK_FOLLOW;

  // Open file with appropriate rights.
  cloudabi_fdstat_t fsb_new = {
      .fs_flags = oflag & 0xfff,
      .fs_rights_base = max & fsb_cur.fs_rights_inheriting,
      .fs_rights_inheriting = fsb_cur.fs_rights_inheriting,
  };
  cloudabi_fd_t newfd;
  error = cloudabi_sys_file_open(lookup, path, strlen(path),
                                 (oflag >> 12) & 0xfff, &fsb_new, &newfd);
  if (error != 0) {
    errno = error;
    return -1;
  }
  return newfd;
}
