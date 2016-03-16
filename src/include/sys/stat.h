// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.

// <sys/stat.h> - data returned by the stat() function
//
// Extensions:
// - S_TYPEISPOLL() and S_TYPEISPROC():
//   Matches descriptors created by kqueue() and pdfork().
// - mkdirat() and mkfifoat():
//   Make mode_t parameter optional, as file permissions do not exist.
//
// Features missing:
// - uid_t, gid_t, struct stat::st_uid, struct stat::st_gid, fchmod(),
//   fchmodat():
//   Filesystem access control management not available.
// - chmod(), lstat(), mkdir(), mkfifo() and stat():
//   Requires global filesystem namespace.

#ifndef _SYS_STAT_H_
#define _SYS_STAT_H_

#include <_/struct/timespec.h>
#include <_/types.h>

#ifndef _DEV_T_DECLARED
typedef __dev_t dev_t;
#define _DEV_T_DECLARED
#endif
#ifndef _INO_T_DECLARED
typedef __ino_t ino_t;
#define _INO_T_DECLARED
#endif
#ifndef _MODE_T_DECLARED
typedef __mode_t mode_t;
#define _MODE_T_DECLARED
#endif
#ifndef _NLINK_T_DECLARED
typedef __nlink_t nlink_t;
#define _NLINK_T_DECLARED
#endif
#ifndef _OFF_T_DECLARED
typedef __off_t off_t;
#define _OFF_T_DECLARED
#endif
#ifndef _TIME_T_DECLARED
typedef __time_t time_t;
#define _TIME_T_DECLARED
#endif

struct stat {
  dev_t st_dev;             // Device ID of device containing file.
  ino_t st_ino;             // File serial number.
  mode_t st_mode;           // Mode of file.
  nlink_t st_nlink;         // Number of hard links to the file.
  off_t st_size;            // The file size in bytes.
  struct timespec st_atim;  // Last data access timestamp.
  struct timespec st_mtim;  // Last data modification timestamp.
  struct timespec st_ctim;  // Last file status change timestamp.
};

#define st_atime st_atim.tv_sec
#define st_ctime st_ctim.tv_sec
#define st_mtime st_mtim.tv_sec

// Old definitions to test for the file type directly. We are unable to
// define S_IFSOCK, because it does not use a single value.
#define S_IFMT 0xff0000   // Type of file.
#define S_IFBLK 0x100000  // Block device.
#define S_IFCHR 0x110000  // Character device.
#define S_IFDIR 0x200000  // Directory.
#define S_IFIFO 0x300000  // FIFO.
#define S_IFLNK 0x900000  // Symbolic link.
#define S_IFREG 0x600000  // Regular file.

// File mode bits. These flags have no effect in this environment, but
// are purely provided to keep standards conformant code compile.
// Normally such flags would be defined as zero, but POSIX requires that
// these flags have a fixed value.
#define S_IXOTH 0x1
#define S_IWOTH 0x2
#define S_IROTH 0x4
#define S_IRWXO (S_IXOTH | S_IWOTH | S_IROTH)
#define S_IXGRP 0x8
#define S_IWGRP 0x10
#define S_IRGRP 0x20
#define S_IRWXG (S_IXGRP | S_IWGRP | S_IRGRP)
#define S_IXUSR 0x40
#define S_IWUSR 0x80
#define S_IRUSR 0x100
#define S_IRWXU (S_IXUSR | S_IWUSR | S_IRUSR)
#define S_ISVTX 0x200
#define S_ISGID 0x400
#define S_ISUID 0x800

// File descriptor types supported by this implementation.
#define S_ISBLK(m) (((m)&S_IFMT) == S_IFBLK)
#define S_ISCHR(m) (((m)&S_IFMT) == S_IFCHR)
#define S_ISDIR(m) (((m)&S_IFMT) == S_IFDIR)
#define S_ISFIFO(m) (((m)&S_IFMT) == S_IFIFO)
#define S_ISLNK(m) (((m)&S_IFMT) == S_IFLNK)
#define S_ISREG(m) (((m)&S_IFMT) == S_IFREG)
#define S_ISSOCK(m) (((m)&0xf00000) == 0x800000)
#define S_TYPEISPOLL(m) (((m)->st_mode & S_IFMT) == 0x400000)
#define S_TYPEISPROC(m) (((m)->st_mode & S_IFMT) == 0x500000)
#define S_TYPEISSHM(m) (((m)->st_mode & S_IFMT) == 0x700000)

// File decriptor types not supported by this implementation.
#define S_TYPEISMQ(m) 0
#define S_TYPEISSEM(m) 0

#define UTIME_NOW (-1)
#define UTIME_OMIT (-2)

__BEGIN_DECLS
int fstat(int, struct stat *);
int fstatat(int, const char *__restrict, struct stat *__restrict, int);
int futimens(int, const struct timespec *);
int mkdirat(int, const char *, ...);
int mkfifoat(int, const char *, ...);
int utimensat(int, const char *, const struct timespec *, int);
__END_DECLS

#endif
