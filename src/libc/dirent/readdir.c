// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/stat.h>

#include <assert.h>
#include <cloudabi_syscalls.h>
#include <dirent.h>
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <cloudlibc_interceptors.h>

#include "dirent_impl.h"

static_assert(DT_BLK == CLOUDABI_FILETYPE_BLOCK_DEVICE, "Value mismatch");
static_assert(DT_CHR == CLOUDABI_FILETYPE_CHARACTER_DEVICE, "Value mismatch");
static_assert(DT_DIR == CLOUDABI_FILETYPE_DIRECTORY, "Value mismatch");
static_assert(DT_FIFO == CLOUDABI_FILETYPE_SOCKET_STREAM, "Value mismatch");
static_assert(DT_LNK == CLOUDABI_FILETYPE_SYMBOLIC_LINK, "Value mismatch");
static_assert(DT_REG == CLOUDABI_FILETYPE_REGULAR_FILE, "Value mismatch");
static_assert(DT_UNKNOWN == CLOUDABI_FILETYPE_UNKNOWN, "Value mismatch");

// Grows a buffer to be large enough to hold a certain amount of data.
#define GROW(buffer, buffer_size, target_size)      \
  do {                                              \
    if ((buffer_size) < (target_size)) {            \
      size_t new_size = (buffer_size);              \
      while (new_size < (target_size))              \
        new_size *= 2;                              \
      void *new_buffer = realloc(buffer, new_size); \
      if (new_buffer == NULL)                       \
        return NULL;                                \
      (buffer) = new_buffer;                        \
      (buffer_size) = new_size;                     \
    }                                               \
  } while (0)

struct dirent *__cloudlibc_readdir(DIR *dirp) {
  for (;;) {
    // Extract the next dirent header.
    size_t buffer_left = dirp->buffer_used - dirp->buffer_processed;
    if (buffer_left < sizeof(cloudabi_dirent_t)) {
      // End-of-file.
      if (dirp->buffer_used < dirp->buffer_size)
        return NULL;
      goto read_entries;
    }
    cloudabi_dirent_t entry;
    memcpy(&entry, dirp->buffer + dirp->buffer_processed, sizeof(entry));

    size_t entry_size = sizeof(cloudabi_dirent_t) + entry.d_namlen;
    if (entry.d_namlen == 0) {
      // Invalid pathname length. Skip the entry.
      dirp->buffer_processed += entry_size;
      continue;
    }

    // The entire entry must be present in buffer space. If not, read
    // the entry another time. Ensure that the read buffer is large
    // enough to fit at least this single entry.
    if (buffer_left < entry_size) {
      GROW(dirp->buffer, dirp->buffer_size, entry_size);
      goto read_entries;
    }

    // Skip entries having null bytes in the filename.
    const char *name = dirp->buffer + dirp->buffer_processed + sizeof(entry);
    if (memchr(name, '\0', entry.d_namlen) != NULL) {
      dirp->buffer_processed += entry_size;
      continue;
    }

    // Return the next directory entry. Ensure that the dirent is large
    // enough to fit the filename.
    GROW(dirp->dirent, dirp->dirent_size,
         offsetof(struct dirent, d_name) + entry.d_namlen + 1);
    struct dirent *dirent = dirp->dirent;
    dirent->d_ino = entry.d_ino;
    dirent->d_type = entry.d_type;
    memcpy(dirent->d_name, name, entry.d_namlen);
    dirent->d_name[entry.d_namlen] = '\0';
    dirp->cookie = entry.d_next;
    dirp->buffer_processed += entry_size;
    return dirent;

  read_entries:
    // Discard data currently stored in the input buffer.
    dirp->buffer_used = dirp->buffer_processed = dirp->buffer_size;

    // Load more directory entries and continue.
    cloudabi_errno_t error =
        cloudabi_sys_file_readdir(dirp->fd, dirp->buffer, dirp->buffer_size,
                                  dirp->cookie, &dirp->buffer_used);
    if (error != 0) {
      errno = error;
      return NULL;
    }
    dirp->buffer_processed = 0;
  }
}

__weak_reference(__cloudlibc_readdir, readdir);
