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

// <sys/types.h> - data types
//
// Extensions:
// - useconds_t:
//   Still used actively, as it was part of POSIX up to issue 6.
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

#ifndef _CLOCK_T_DECLARED
typedef __clock_t clock_t;
#define _CLOCK_T_DECLARED
#endif
#ifndef _CLOCKID_T_DECLARED
typedef __clockid_t clockid_t;
#define _CLOCKID_T_DECLARED
#endif
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
#ifndef _PTHREAD_ATTR_T_DECLARED
typedef __pthread_attr_t pthread_attr_t;
#define _PTHREAD_ATTR_T_DECLARED
#endif
#ifndef _PTHREAD_BARRIER_T_DECLARED
typedef __pthread_barrier_t pthread_barrier_t;
#define _PTHREAD_BARRIER_T_DECLARED
#endif
#ifndef _PTHREAD_BARRIERATTR_T_DECLARED
typedef __pthread_barrierattr_t pthread_barrierattr_t;
#define _PTHREAD_BARRIERATTR_T_DECLARED
#endif
#ifndef _PTHREAD_COND_T_DECLARED
typedef __pthread_cond_t pthread_cond_t;
#define _PTHREAD_COND_T_DECLARED
#endif
#ifndef _PTHREAD_CONDATTR_T_DECLARED
typedef __pthread_condattr_t pthread_condattr_t;
#define _PTHREAD_CONDATTR_T_DECLARED
#endif
#ifndef _PTHREAD_KEY_T_DECLARED
typedef __pthread_key_t pthread_key_t;
#define _PTHREAD_KEY_T_DECLARED
#endif
#ifndef _PTHREAD_MUTEX_T_DECLARED
typedef __pthread_lock_t pthread_mutex_t;
#define _PTHREAD_MUTEX_T_DECLARED
#endif
#ifndef _PTHREAD_MUTEXATTR_T_DECLARED
typedef __pthread_lockattr_t pthread_mutexattr_t;
#define _PTHREAD_MUTEXATTR_T_DECLARED
#endif
#ifndef _PTHREAD_ONCE_T_DECLARED
typedef __pthread_once_t pthread_once_t;
#define _PTHREAD_ONCE_T_DECLARED
#endif
#ifndef _PTHREAD_RWLOCK_T_DECLARED
typedef __pthread_lock_t pthread_rwlock_t;
#define _PTHREAD_RWLOCK_T_DECLARED
#endif
#ifndef _PTHREAD_RWLOCKATTR_T_DECLARED
typedef __pthread_lockattr_t pthread_rwlockattr_t;
#define _PTHREAD_RWLOCKATTR_T_DECLARED
#endif
#ifndef _PTHREAD_SPINLOCK_T_DECLARED
typedef __pthread_lock_t pthread_spinlock_t;
#define _PTHREAD_SPINLOCK_T_DECLARED
#endif
#ifndef _PTHREAD_T_DECLARED
typedef __pthread_t pthread_t;
#define _PTHREAD_T_DECLARED
#endif
#ifndef _SIZE_T_DECLARED
typedef __size_t size_t;
#define _SIZE_T_DECLARED
#endif
#ifndef _SSIZE_T_DECLARED
typedef __ssize_t ssize_t;
#define _SSIZE_T_DECLARED
#endif
#ifndef _SUSECONDS_T_DECLARED
typedef __suseconds_t suseconds_t;
#define _SUSECONDS_T_DECLARED
#endif
#ifndef _TIME_T_DECLARED
typedef __time_t time_t;
#define _TIME_T_DECLARED
#endif
#ifndef _USECONDS_T_DECLARED
typedef __useconds_t useconds_t;
#define _USECONDS_T_DECLARED
#endif

#endif
