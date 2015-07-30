// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalls.h>

#include <sys/stat.h>

#include <dirent.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>

#include "dirent_impl.h"

static_assert(DT_BLK == CLOUDABI_FILETYPE_BLOCK_DEVICE << 16, "Value mismatch");
static_assert(DT_CHR == CLOUDABI_FILETYPE_CHARACTER_DEVICE << 16,
              "Value mismatch");
static_assert(DT_DIR == CLOUDABI_FILETYPE_DIRECTORY << 16, "Value mismatch");
static_assert(DT_FIFO == CLOUDABI_FILETYPE_FIFO << 16, "Value mismatch");
static_assert(DT_LNK == CLOUDABI_FILETYPE_SYMBOLIC_LINK << 16,
              "Value mismatch");
static_assert(DT_REG == CLOUDABI_FILETYPE_REGULAR_FILE << 16, "Value mismatch");
static_assert(DT_UNKNOWN == CLOUDABI_FILETYPE_UNKNOWN << 16, "Value mismatch");

static_assert(S_ISBLK(DT_BLK), "Value mismatch");
static_assert(S_ISCHR(DT_CHR), "Value mismatch");
static_assert(S_ISDIR(DT_DIR), "Value mismatch");
static_assert(S_ISFIFO(DT_FIFO), "Value mismatch");
static_assert(S_ISLNK(DT_LNK), "Value mismatch");
static_assert(S_ISREG(DT_REG), "Value mismatch");

static bool fetch_first_entry(DIR *dirp) {
  // Extract the next dirent header.
  size_t buffer_left = dirp->buffer_length - dirp->buffer_processed;
  if (buffer_left <= sizeof(cloudabi_dirent_t))
    return false;
  cloudabi_dirent_t dirent;
  memcpy(&dirent, dirp->buffer + dirp->buffer_processed, sizeof(dirent));

  // Bounds checking on the length of the filename.
  if (dirent.d_namlen == 0 || dirent.d_namlen > NAME_MAX)
    return false;

  // Ensure that the entire filename is present in the buffer space.
  size_t dirent_length = sizeof(cloudabi_dirent_t) + dirent.d_namlen;
  if (buffer_left < dirent_length)
    return false;

  // Filename cannot contain a null byte.
  const char *name = dirp->buffer + dirp->buffer_processed + sizeof(dirent);
  if (memchr(name, '\0', dirent.d_namlen) != NULL)
    return false;

  dirp->dirent.d_ino = dirent.d_ino;
  dirp->dirent.d_type = dirent.d_type << 16;
  strlcpy(dirp->dirent.d_name, name, dirent.d_namlen + 1);

  dirp->cookie = dirent.d_next;
  dirp->buffer_processed += dirent_length;
  return true;
}

struct dirent *readdir(DIR *dirp) {
  for (;;) {
    // See if we can extract an entry from internal buffer space.
    if (fetch_first_entry(dirp))
      return &dirp->dirent;

    // Don't attempt to fetch new entries if we've already reached
    // end-of-file.
    if (dirp->buffer_length < sizeof(dirp->buffer))
      return NULL;

    // Corner case: if we've failed to fetch an entry from the internal
    // buffer space, even though the entry was placed at the start, it
    // means that the entry is invalid (e.g., empty pathname, pathname
    // too long for this implementation to support). Skip this file and
    // continue processing the next.
    if (dirp->buffer_processed == 0)
      dirp->cookie = dirp->first_dirent.d_next;

    // Load next directory entries.
    cloudabi_errno_t error =
        cloudabi_sys_file_readdir(dirp->fd, dirp->buffer, sizeof(dirp->buffer),
                                  dirp->cookie, &dirp->buffer_length);
    if (error != 0) {
      errno = error;
      return NULL;
    }
    dirp->buffer_processed = 0;
  }
}
