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

// <sys/mman.h> - memory management declarations
//
// Extensions:
// - SHM_ANON:
//   Anonymous shared memory objects. Also supported by FreeBSD.
// - MAP_ANON and MAP_ANONYMOUS:
//   Present on most operating systems.
// - shm_open():
//   Make mode_t parameter optional, as file permissions do not exist.
//
// Features missing:
// - Typed Memory Objects:
//   Not applicable.
// - MCL_CURRENT, MCL_FUTURE, mlock(), mlockall(), munlock() and munlockall():
//   Not meaningful in unprivileged contexts.
// - shm_unlink():
//   Requires global filesystem namespace.

#ifndef _SYS_MMAN_H_
#define _SYS_MMAN_H_

#include <_/types.h>

#define PROT_NONE 0x0
#define PROT_EXEC 0x1
#define PROT_WRITE 0x2
#define PROT_READ 0x4

#define MAP_ANON 0x1
#define MAP_FIXED 0x2
#define MAP_PRIVATE 0x4
#define MAP_SHARED 0x8

// For compatibility.
#define MAP_ANONYMOUS MAP_ANON

#define MS_ASYNC 0x1
#define MS_INVALIDATE 0x2
#define MS_SYNC 0x4

#define MAP_FAILED ((void *)-1)

#define POSIX_MADV_DONTNEED 1
#define POSIX_MADV_NORMAL 3
#define POSIX_MADV_RANDOM 4
#define POSIX_MADV_SEQUENTIAL 5
#define POSIX_MADV_WILLNEED 6

#define SHM_ANON ((char *)-1)

#ifndef _MODE_T_DECLARED
typedef __mode_t mode_t;
#define _MODE_T_DECLARED
#endif
#ifndef _OFF_T_DECLARED
typedef __off_t off_t;
#define _OFF_T_DECLARED
#endif
#ifndef _SIZE_T_DECLARED
typedef __size_t size_t;
#define _SIZE_T_DECLARED
#endif

__BEGIN_DECLS
void *mmap(void *, size_t, int, int, int, off_t);
int mprotect(void *, size_t, int);
int msync(void *, size_t, int);
int munmap(void *, size_t);
int posix_madvise(void *, size_t, int);
int shm_open(const char *, int, ...);
__END_DECLS

#endif
