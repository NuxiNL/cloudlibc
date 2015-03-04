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

// <sys/types.h> - data types
//
// Features missing:
// - fsblkcnt_t and fsfilcnt_t:
//   Filesystem-level statistics not available.
// - gid_t and uid_t:
//   POSIX credential management is not available.
// - id_t and pid_t:
//   Requires global process namespace.
// - timer_t:
//   Signal handling not available.

#ifndef _SYS_TYPES_H_
#define _SYS_TYPES_H_

#include <_/types.h>

typedef __clock_t clock_t;
typedef __clockid_t clockid_t;
typedef __dev_t dev_t;
typedef __ino_t ino_t;
typedef __mode_t mode_t;
typedef __nlink_t nlink_t;
typedef __off_t off_t;
typedef __pthread_attr_t pthread_attr_t;
typedef __pthread_barrier_t pthread_barrier_t;
typedef __pthread_barrierattr_t pthread_barrierattr_t;
typedef __pthread_cond_t pthread_cond_t;
typedef __pthread_condattr_t pthread_condattr_t;
typedef __pthread_key_t pthread_key_t;
typedef __pthread_lock_t pthread_mutex_t;
typedef __pthread_lockattr_t pthread_mutexattr_t;
typedef __pthread_once_t pthread_once_t;
typedef __pthread_lock_t pthread_rwlock_t;
typedef __pthread_lockattr_t pthread_rwlockattr_t;
typedef __pthread_lock_t pthread_spinlock_t;
typedef __pthread_t pthread_t;
typedef __size_t size_t;
typedef __ssize_t ssize_t;
typedef __time_t time_t;

#endif
