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

// <mqueue.h> - message queues
//
// Extensions:
// - mq_destroy() and mq_init():
//   Allows for the creation of anonymous process-local message queues.
//
// Features missing:
// - struct sigevent and mq_notify():
//   Requires signal handling support.
// - mq_close(), mq_open() and mq_unlink():
//   Requires global IPC namespace. Omitting mq_open() also means that
//   mqd_t does not need to be an arithmetic type.

#ifndef _MQUEUE_H_
#define _MQUEUE_H_

#include <_/struct/timespec.h>  // IWYU pragma: export
#include <_/types.h>

#ifndef _PTHREAD_ATTR_T_DECLARED
typedef __pthread_attr_t pthread_attr_t;
#define _PTHREAD_ATTR_T_DECLARED
#endif
#ifndef _SIZE_T_DECLARED
typedef __size_t size_t;
#define _SIZE_T_DECLARED
#endif
#ifndef _SSIZE_T_DECLARED
typedef __ssize_t ssize_t;
#define _SSIZE_T_DECLARED
#endif

typedef struct {
  struct __mqd *__mqd;
} mqd_t;

struct mq_attr {
  long mq_flags;    // Message queue flags.
  long mq_maxmsg;   // Maximum number of messages.
  long mq_msgsize;  // Maximum message size.
  long mq_curmsgs;  // Number of messages currently queued.
};

__BEGIN_DECLS
int mq_destroy(mqd_t);
int mq_getattr(mqd_t, struct mq_attr *);
int mq_init(mqd_t *, const struct mq_attr *);
ssize_t mq_receive(mqd_t, char *, size_t, unsigned int *);
int mq_send(mqd_t, const char *, size_t, unsigned int);
int mq_setattr(mqd_t, const struct mq_attr *__restrict,
               struct mq_attr *__restrict);
ssize_t mq_timedreceive(mqd_t, char *__restrict, size_t,
                        unsigned int *__restrict,
                        const struct timespec *__restrict);
int mq_timedsend(mqd_t, const char *, size_t, unsigned int,
                 const struct timespec *);
__END_DECLS

#endif
