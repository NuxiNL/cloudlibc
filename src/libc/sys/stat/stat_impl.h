// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#ifndef SYS_STAT_STAT_IMPL_H
#define SYS_STAT_STAT_IMPL_H

#include <common/syscalldefs.h>
#include <common/time.h>

#include <sys/stat.h>

#include <assert.h>
#include <stdbool.h>

#define TRANSLATE_FILETYPE(type) ((mode_t)(type) << 16)

static_assert(S_ISBLK(TRANSLATE_FILETYPE(CLOUDABI_FILETYPE_BLOCK_DEVICE)),
              "Value mismatch");
static_assert(S_ISCHR(TRANSLATE_FILETYPE(CLOUDABI_FILETYPE_CHARACTER_DEVICE)),
              "Value mismatch");
static_assert(S_ISDIR(TRANSLATE_FILETYPE(CLOUDABI_FILETYPE_DIRECTORY)),
              "Value mismatch");
static_assert(S_ISFIFO(TRANSLATE_FILETYPE(CLOUDABI_FILETYPE_FIFO)),
              "Value mismatch");
static_assert(S_TYPEISPOLL(TRANSLATE_FILETYPE(CLOUDABI_FILETYPE_POLL)),
              "Value mismatch");
static_assert(S_TYPEISPROC(TRANSLATE_FILETYPE(CLOUDABI_FILETYPE_PROCESS)),
              "Value mismatch");
static_assert(S_ISREG(TRANSLATE_FILETYPE(CLOUDABI_FILETYPE_REGULAR_FILE)),
              "Value mismatch");
static_assert(S_TYPEISSHM(TRANSLATE_FILETYPE(CLOUDABI_FILETYPE_SHARED_MEMORY)),
              "Value mismatch");
static_assert(S_ISSOCK(TRANSLATE_FILETYPE(CLOUDABI_FILETYPE_SOCKET_DGRAM)),
              "Value mismatch");
static_assert(S_ISSOCK(TRANSLATE_FILETYPE(CLOUDABI_FILETYPE_SOCKET_SEQPACKET)),
              "Value mismatch");
static_assert(S_ISSOCK(TRANSLATE_FILETYPE(CLOUDABI_FILETYPE_SOCKET_STREAM)),
              "Value mismatch");
static_assert(S_ISLNK(TRANSLATE_FILETYPE(CLOUDABI_FILETYPE_SYMBOLIC_LINK)),
              "Value mismatch");

static inline void to_public_stat(const cloudabi_filestat_t *in,
                                  struct stat *out) {
  *out = (struct stat){
#define COPY_FIELD(field) .field = in->field
#define COPY_TIMESPEC(field) .field = timestamp_to_timespec(in->field)
      COPY_FIELD(st_dev),
      COPY_FIELD(st_ino),
      .st_mode = TRANSLATE_FILETYPE(in->st_filetype),
      COPY_FIELD(st_nlink),
      COPY_FIELD(st_size),
      COPY_TIMESPEC(st_atim),
      COPY_TIMESPEC(st_mtim),
      COPY_TIMESPEC(st_ctim),
#undef COPY_FIELD
#undef COPY_TIMESPEC
  };
}

static inline bool utimens_get_timestamps(const struct timespec *times,
                                          cloudabi_filestat_t *fs,
                                          cloudabi_fsflags_t *flags) {
  if (times == NULL) {
    // Update both timestamps.
    *flags = CLOUDABI_FILESTAT_ATIM_NOW | CLOUDABI_FILESTAT_MTIM_NOW;
  } else {
    // Set individual timestamps.
    *flags = 0;
    switch (times[0].tv_nsec) {
      case UTIME_NOW:
        *flags |= CLOUDABI_FILESTAT_ATIM_NOW;
        break;
      case UTIME_OMIT:
        break;
      default:
        *flags |= CLOUDABI_FILESTAT_ATIM;
        if (!timespec_to_timestamp_exact(&times[0], &fs->st_atim))
          return false;
        break;
    }

    switch (times[1].tv_nsec) {
      case UTIME_NOW:
        *flags |= CLOUDABI_FILESTAT_MTIM_NOW;
        break;
      case UTIME_OMIT:
        break;
      default:
        *flags |= CLOUDABI_FILESTAT_MTIM;
        if (!timespec_to_timestamp_exact(&times[1], &fs->st_mtim))
          return false;
        break;
    }
  }
  return true;
}

#endif
