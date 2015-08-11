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

#ifndef ___TIME_SELECT_H_
#define ___TIME_SELECT_H_

#include <_/types.h>

#ifndef _SUSECONDS_T_DECLARED
typedef __suseconds_t suseconds_t;
#define _SUSECONDS_T_DECLARED
#endif
#ifndef _TIME_T_DECLARED
typedef __time_t time_t;
#define _TIME_T_DECLARED
#endif

struct timeval {
  time_t tv_sec;        // Seconds.
  suseconds_t tv_usec;  // Microseconds.
};

// File descriptor sets for select().
//
// Most systems implement file descriptor sets as a bitmask, for the
// reason that they have high density and allow for FD_CLR(), FD_ISSET()
// and FD_SET() to run in constant time.
//
// For CloudABI we use an array of file descriptor numbers. The
// advantage of this approach is that these functions are safe to use,
// even if the file descriptor numbers exceed FD_SETSIZE.
//
// It is expected that the additional overhead of FD_CLR(), FD_ISSET()
// and FD_SET() is compensated by the fact that FD_ZERO() runs in
// constant time and that select() does not need to scan potentially
// sparse bitmasks.

#define FD_SETSIZE 1024

typedef struct {
  __size_t __nfds;
  int __fds[FD_SETSIZE];
} fd_set;

static __inline void _FD_CLR(int __fd, fd_set *__fd_set) {
  for (__size_t __i = 0; __i < __fd_set->__nfds; ++__i) {
    if (__fd_set->__fds[__i] == __fd) {
      // Keep set contiguous.
      __fd_set->__fds[__i] = __fd_set->__fds[--__fd_set->__nfds];
      return;
    }
  }
}

static __inline int _FD_ISSET(int __fd, fd_set *__fd_set) {
  for (__size_t __i = 0; __i < __fd_set->__nfds; ++__i)
    if (__fd_set->__fds[__i] == __fd)
      return 1;
  return 0;
}

static __inline void _FD_SET(int __fd, fd_set *__fd_set) {
  // Prevent set overflows.
  if (__fd_set->__nfds >= __arraycount(__fd_set->__fds))
    return;
  // Prevent duplicate entries.
  for (__size_t __i = 0; __i < __fd_set->__nfds; ++__i)
    if (__fd_set->__fds[__i] == __fd)
      return;
  __fd_set->__fds[__fd_set->__nfds++] = __fd;
}

static __inline void _FD_ZERO(fd_set *__fd_set) {
  __fd_set->__nfds = 0;
}

__BEGIN_DECLS
void FD_CLR(int, fd_set *);
int FD_ISSET(int, fd_set *);
void FD_SET(int, fd_set *);
void FD_ZERO(fd_set *);
int select(int, fd_set *__restrict, fd_set *__restrict, fd_set *__restrict,
           struct timeval *__restrict);
__END_DECLS

#define FD_CLR(fd, fds) _FD_CLR(fd, fds)
#define FD_ISSET(fd, fds) _FD_ISSET(fd, fds)
#define FD_SET(fd, fds) _FD_SET(fd, fds)
#define FD_ZERO(fds) _FD_ZERO(fds)

#endif
