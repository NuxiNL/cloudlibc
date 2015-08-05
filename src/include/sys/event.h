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
    struct kevent *__ke = (ke);                                           \
    (__ke)->ident = (v_ident);                                            \
    (__ke)->filter = (v_filter);                                          \
    (__ke)->flags = (v_flags);                                            \
    (__ke)->fflags = (v_fflags);                                          \
    (__ke)->data = (v_data);                                              \
    (__ke)->udata = (v_udata);                                            \
  } while (0)

#define EV_ADD 0x1       // Add the event to the kqueue.
#define EV_CLEAR 0x2     // Reset event to initial state.
#define EV_DELETE 0x4    // Removes the event from the kqueue.
#define EV_DISABLE 0x8   // Disable reporting through kqueue().
#define EV_ENABLE 0x10   // Enable reporting through kqueue().
#define EV_ONESHOT 0x20  // Only report first occurrence.

#define EV_EOF 0x4000    // Filter-specific end-of-file condition.
#define EV_ERROR 0x8000  // Error has occurred.

#define EVFILT_READ 3   // Data available to read from descriptor.
#define EVFILT_WRITE 4  // Possibility to write to descriptor.

__BEGIN_DECLS
__ssize_t kevent(int, const struct kevent *, __size_t, struct kevent *,
                 __size_t, const struct timespec *);
int kqueue(void);
__END_DECLS

#endif
