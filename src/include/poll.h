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

// <poll.h> - definitions for the poll() function
//
// Features missing:
// - POLLRDBAND, POLLPRI, POLLWRBAND:
//   Out-of-band data is not supported.

#ifndef _POLL_H_
#define _POLL_H_

#include <_/types.h>

typedef __size_t nfds_t;

struct pollfd {
  int fd;         // The descriptor being polled.
  short events;   // The input event flags.
  short revents;  // The output event flags.
};

// Input/output flags.
#define POLLRDNORM 0x1  // Normal data may be read without blocking.
#define POLLWRNORM 0x2  // Normal data may be written without blocking.

// Output flags.
#define POLLERR 0x1000   // An error has occurred.
#define POLLHUP 0x2000   // Device has been disconnected.
#define POLLNVAL 0x4000  // Invalid fd member.

#define POLLIN POLLRDNORM
#define POLLOUT POLLWRNORM

__BEGIN_DECLS
int poll(struct pollfd *, __size_t, int);
__END_DECLS

#endif
