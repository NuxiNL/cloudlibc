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

// Machine dependent definitions.

typedef void CLOUDABI_IDENT(threadentry_t)(cloudabi_tid_t, CLOUDABI_PTR(void));

typedef struct {
  CLOUDABI_PTR(const void) iov_base;
  CLOUDABI_IDENT(size_t) iov_len;
} CLOUDABI_IDENT(ciovec_t);

typedef struct {
  CLOUDABI_PTR(void) userdata;
  cloudabi_errno_t error;
  uint8_t type;
  union {
    // CLOUDABI_EVENT_TYPE_CLOCK: Wait until the value of a clock
    // exceeds a value.
    struct {
      cloudabi_clockid_t clock_id;
      cloudabi_timestamp_t timeout;
      cloudabi_timestamp_t precision;
    } clock;

    // CLOUDABI_EVENT_TYPE_CONDVAR: Release a lock and wait on a
    // condition variable.
    struct {
      CLOUDABI_PTR(_Atomic(cloudabi_condvar_t)) condvar;
      CLOUDABI_PTR(_Atomic(cloudabi_lock_t)) lock;
    } condvar;

    // CLOUDABI_EVENT_TYPE_FD_READ and CLOUDABI_EVENT_TYPE_FD_WRITE:
    // Wait for a file descriptor to allow read() and write() to be
    // called without blocking.
    struct {
      cloudabi_fd_t fd;
    } fd_readwrite;

    // CLOUDABI_EVENT_LOCK_RDLOCK and CLOUDABI_EVENT_LOCK_WRLOCK: Wait
    // and acquire a read or write lock.
    struct {
      CLOUDABI_PTR(_Atomic(cloudabi_lock_t)) lock;
    } lock;

    // CLOUDABI_EVENT_TYPE_PROC_TERMINATE: Wait for a process to terminate.
    struct {
      cloudabi_fd_t fd;
      cloudabi_signal_t signal;      // Non-zero if process got killed.
      cloudabi_exitcode_t exitcode;  // Exit code.
    } proc_terminate;
  };
} CLOUDABI_IDENT(event_t);

typedef struct {
  CLOUDABI_PTR(void) iov_base;
  CLOUDABI_IDENT(size_t) iov_len;
} CLOUDABI_IDENT(iovec_t);

typedef struct {
  CLOUDABI_PTR(const CLOUDABI_IDENT(iovec_t)) ri_data;  // Data I/O vectors.
  CLOUDABI_IDENT(size_t) ri_datalen;   // Number of data I/O vectors.
  CLOUDABI_PTR(cloudabi_fd_t) ri_fds;  // File descriptors.
  CLOUDABI_IDENT(size_t) ri_fdslen;    // Number of file descriptors.
  cloudabi_msgflags_t ri_flags;        // Input flags.
} CLOUDABI_IDENT(recv_in_t);

typedef struct {
  CLOUDABI_IDENT(size_t) ro_datalen;  // Bytes of data received.
  CLOUDABI_IDENT(size_t) ro_fdslen;   // Number of file descriptors received.
  cloudabi_sockaddr_t ro_sockname;    // Address of receiver.
  cloudabi_sockaddr_t ro_peername;    // Address of sender.
  cloudabi_msgflags_t ro_flags;       // Output flags.
} CLOUDABI_IDENT(recv_out_t);

typedef struct {
  CLOUDABI_PTR(const CLOUDABI_IDENT(ciovec_t)) si_data;  // Data I/O vectors.
  CLOUDABI_IDENT(size_t) si_datalen;         // Number of data I/O vectors.
  CLOUDABI_PTR(const cloudabi_fd_t) si_fds;  // File descriptors.
  CLOUDABI_IDENT(size_t) si_fdslen;          // Number of file descriptors.
  cloudabi_msgflags_t si_flags;              // Input flags.
} CLOUDABI_IDENT(send_in_t);

typedef struct {
  CLOUDABI_IDENT(size_t) so_datalen;  // Bytes of data sent.
} CLOUDABI_IDENT(send_out_t);

typedef struct {
  CLOUDABI_PTR(CLOUDABI_IDENT(threadentry_t)) entry_point;  // Entry point.
  CLOUDABI_PTR(void) stack;           // Pointer to stack buffer.
  CLOUDABI_IDENT(size_t) stack_size;  // Size of stack buffer.
  CLOUDABI_PTR(void) argument;        // Argument to be passed to entry point.
} CLOUDABI_IDENT(threadattr_t);
