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

// <sys/event.h> - event notification

#ifndef _SYS_EVENT_H_
#define _SYS_EVENT_H_

#include <_/types.h>

struct timespec;

struct kevent {
  __uintptr_t ident;     // Identifier for this event.
  short filter;          // Filter for event.
  unsigned short flags;  // Action flags for kqueue.
  unsigned int fflags;   // Filter flag value.
  __intptr_t data;       // Filter data value.
  void *udata;           // Opaque user data identifier.
};

#define EV_SET(ke, v_ident, v_filter, v_flags, v_fflags, v_data, v_udata) \
  do {                                                                    \
    (ke)->ident = (v_ident);                                              \
    (ke)->filter = (v_filter);                                            \
    (ke)->flags = (v_flags);                                              \
    (ke)->fflags = (v_fflags);                                            \
    (ke)->data = (v_data);                                                \
    (ke)->udata = (v_udata);                                              \
  } while (0)

#define EV_ADD 0x1
#define EV_CLEAR 0x2
#define EV_DELETE 0x4
#define EV_DISABLE 0x8
#define EV_DISPATCH 0x10
#define EV_ENABLE 0x20
#define EV_EOF 0x40
#define EV_ERROR 0x80
#define EV_ONESHOT 0x100
#define EV_RECEIPT 0x200

#define EVFILT_AIO 1
#define EVFILT_PROCDESC 2
#define EVFILT_READ 3
#define EVFILT_WRITE 4

__BEGIN_DECLS
int kevent(int, const struct kevent *, int, struct kevent *, int,
           const struct timespec *);
int kqueue(void);
__END_DECLS

#endif
