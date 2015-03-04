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

#ifndef _MQUEUE_H_
#define _MQUEUE_H_

#include <_/struct/timespec.h>
#include <_/types.h>

typedef struct __mqd *mqd_t;
typedef __pthread_attr_t pthread_attr_t;
typedef __size_t size_t;
typedef __ssize_t ssize_t;

struct mq_attr {
  long mq_flags;    // Message queue flags.
  long mq_maxmsg;   // Maximum number of messages.
  long mq_msgsize;  // Maximum message size.
  long mq_curmsgs;  // Number of messages currently queued.
};

#define MQ_ANON ((char *)-1)

__BEGIN_DECLS
int mq_close(mqd_t);
int mq_getattr(mqd_t, struct mq_attr *);
mqd_t mq_open(const char *, int, ...);
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
