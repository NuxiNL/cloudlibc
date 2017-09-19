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

// <aio.h> - asynchronous input and output
//
// Features missing:
// - struct aiocb::aio_sigevent and lio_listio():
//   Signal handling is not available. The last parameter of
//   lio_listio() is optional.
// - struct aiocb::aio_reqprio:
//   I/O priorities not supported.

#ifndef _AIO_H_
#define _AIO_H_

#include <_/struct/timespec.h>  // IWYU pragma: export
#include <_/types.h>

#ifndef _OFF_T_DECLARED
typedef __off_t off_t;
#define _OFF_T_DECLARED
#endif
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

struct aiocb {
  int aio_fildes;          // File descriptor.
  off_t aio_offset;        // File offset.
  volatile void *aio_buf;  // Location of buffer.
  size_t aio_nbytes;       // Length of transfer.
  int aio_lio_opcode;      // Operation to be performed.

  ssize_t __aio_return;  // Negative errno on failure, success otherwise.
};

// Values returned by aio_cancel().
#define AIO_ALLDONE 1      // No operations cancelled; all completed.
#define AIO_CANCELED 2     // All operations have been cancelled.
#define AIO_NOTCANCELED 3  // Some operations have not been cancelled.

// Values for struct aiocb::aio_lio_opcode.
#define LIO_NOP 1    // Ignore.
#define LIO_READ 2   // Read request.
#define LIO_WRITE 3  // Write request.

// Flags for lio_listio().
#define LIO_NOWAIT 1  // Return immediately.
#define LIO_WAIT 2    // Wait until all operations have completed.

__BEGIN_DECLS
int aio_cancel(int, struct aiocb *);
int aio_error(const struct aiocb *);
int aio_fsync(int, struct aiocb *);
int aio_read(struct aiocb *);
ssize_t aio_return(struct aiocb *);
int aio_suspend(const struct aiocb *const *, int, const struct timespec *);
int aio_write(struct aiocb *);
int lio_listio(int, struct aiocb *__restrict const *__restrict, int, ...);
__END_DECLS

#endif
