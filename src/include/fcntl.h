// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
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

// <fcntl.h> - file control options
//
// Extensions:
// - O_NDELAY:
//   Identical to O_NONBLOCK.
//
// Features missing:
// - F_DUPFD and F_DUPFD_CLOEXEC:
//   Ordering of file descriptors should not be relevant.
// - F_GETLK, F_SETLK, F_SETLKW, F_RDLCK, F_UNLCK, F_WRLCK, pid_t and
//   struct flock:
//   File locking not available.
// - F_GETOWN and F_SETOWN:
//   Signal handling not available.
// - O_TTY_INIT:
//   TTY handling not available.
// - creat() and open():
//   Requires global filesystem namespace.

#ifndef _FCNTL_H_
#define _FCNTL_H_

#include <_/types.h>

#ifndef _MODE_T_DECLARED
typedef __mode_t mode_t;
#define _MODE_T_DECLARED
#endif
#ifndef _OFF_T_DECLARED
typedef __off_t off_t;
#define _OFF_T_DECLARED
#endif

// fcntl() options.
#define F_GETFD 1
#define F_SETFD 2
#define F_GETFL 3
#define F_SETFL 4

// For fcntl()'s F_GETFD and F_SETFD.
#define FD_CLOEXEC 0x1

// First twelve bits: flags that remain in effect after opening.
#define O_APPEND 0x1
#define O_DSYNC 0x2
#define O_NONBLOCK 0x4
#define O_RSYNC 0x8
#define O_SYNC 0x10

// Second twelve bits: flags that only have effect while opening.
#define O_CREAT 0x1000
#define O_DIRECTORY 0x2000
#define O_EXCL 0x4000
#define O_TRUNC 0x8000

// Last eight bits: flags that are handled by openat() internally.
#define O_NOFOLLOW 0x1000000
#define O_EXEC 0x2000000
#define O_RDONLY 0x4000000
#define O_SEARCH 0x8000000
#define O_WRONLY 0x10000000

// For compatibility.
#define O_CLOEXEC 0          // Has no effect.
#define O_NDELAY O_NONBLOCK  // Different spelling.
#define O_NOCTTY 0           // Has no effect.

// Access modes.
#define O_ACCMODE (O_EXEC | O_RDONLY | O_SEARCH | O_WRONLY)
#define O_RDWR (O_RDONLY | O_WRONLY)

// Flags for *at().
#define AT_EACCESS 0
#define AT_SYMLINK_NOFOLLOW 0x1
#define AT_SYMLINK_FOLLOW 0x2
#define AT_REMOVEDIR 0x4

// Flags for posix_fadvise().
#define POSIX_FADV_DONTNEED 1
#define POSIX_FADV_NOREUSE 2
#define POSIX_FADV_NORMAL 3
#define POSIX_FADV_RANDOM 4
#define POSIX_FADV_SEQUENTIAL 5
#define POSIX_FADV_WILLNEED 6

__BEGIN_DECLS
int fcntl(int, int, ...);
int openat(int, const char *, int, ...);
int posix_fadvise(int, off_t, off_t, int);
int posix_fallocate(int, off_t, off_t);
__END_DECLS

#endif
