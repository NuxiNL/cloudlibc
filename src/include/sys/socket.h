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

// <sys/socket.h> - main sockets header
//
// Extensions:
// - bindat(), connectat():
//   Allows for binding of UNIX domain sockets without requiring the
//   global filesystem namespace. Also present on FreeBSD, though having
//   a different prototype.
// - CMSG_LEN() and CMSG_SPACE():
//   Part of RFC 2292.
//
// Features missing:
// - SO_OOBINLINE, MSG_OOB and sockatmark():
//   Sending and receiving out-of-band data is unsupported.
// - bind(), connect() and sendto():
//   Requires global network address space.
// - setsockopt():
//   Socket parameters cannot be adjusted.

#ifndef _SYS_SOCKET_H_
#define _SYS_SOCKET_H_

#include <_/struct/iovec.h>
#include <_/limits.h>
#include <_/types.h>

#ifndef _SA_FAMILY_T_DECLARED
typedef __sa_family_t sa_family_t;
#define _SA_FAMILY_T_DECLARED
#endif
#ifndef _SOCKLEN_T_DECLARED
typedef __size_t socklen_t;
#define _SOCKLEN_T_DECLARED
#endif
#ifndef _SIZE_T_DECLARED
typedef __size_t size_t;
#define _SIZE_T_DECLARED
#endif
#ifndef _SSIZE_T_DECLARED
typedef __ssize_t ssize_t;
#define _SSIZE_T_DECLARED
#endif

struct sockaddr {
  _Alignas(__max_align_t) sa_family_t sa_family;
  char sa_data[1];
};

struct sockaddr_storage {
  _Alignas(__max_align_t) sa_family_t ss_family;
  char __ss_data[32];
};

struct msghdr {
  void *msg_name;         // Optional address.
  size_t msg_namelen;     // Size of address.
  struct iovec *msg_iov;  // Scatter/gather array.
  int msg_iovlen;         // Members in msg_iov.
  void *msg_control;      // Ancillary data.
  size_t msg_controllen;  // Ancillary data buffer length.
  int msg_flags;          // Flags on received message.
};

struct cmsghdr {
  size_t cmsg_len;  // Data byte count, including the cmsghdr.
  int cmsg_level;   // Originating protocol.
  int cmsg_type;    // Protocol-specific type.
  _Alignas(__max_align_t) unsigned char __cmsg_data[1];  // Data.
};

#define SOCK_DGRAM 0x80      // Datagram socket.
#define SOCK_SEQPACKET 0x81  // Sequenced-packet socket.
#define SOCK_STREAM 0x82     // Byte-stream socket.

#define SOL_SOCKET _INT_MAX  // Options to be accessed at socket level.

#define SCM_RIGHTS 1  // File descriptor passing for UNIX sockets.

// Utility macros.
#define _CMSG_ALIGN(ptr) \
  ((struct cmsghdr *)__roundup((__uintptr_t)(ptr), _Alignof(struct cmsghdr)))
#define _CMSG_VALIDATE(mhdr, cmsg)                                       \
  (CMSG_DATA(cmsg) <=                                                    \
           (unsigned char *)(mhdr)->msg_control + (mhdr)->msg_controllen \
       ? (cmsg)                                                          \
       : (struct cmsghdr *)NULL)

#define CMSG_DATA(cmsg) (&(cmsg)->__cmsg_data[0])
#define CMSG_LEN(len) ((__size_t)__offsetof(struct cmsghdr, __cmsg_data[len]))
#define CMSG_NXTHDR(mhdr, cmsg)                                            \
  ((cmsg) == NULL ? CMSG_FIRSTHDR(mhdr)                                    \
                  : _CMSG_VALIDATE(mhdr, _CMSG_ALIGN((__uintptr_t)(cmsg) + \
                                                     (cmsg)->cmsg_len)))
#define CMSG_FIRSTHDR(mhdr) \
  _CMSG_VALIDATE(mhdr, _CMSG_ALIGN((mhdr)->msg_control))
#define CMSG_SPACE(len) (CMSG_LEN(len) + _Alignof(struct cmsghdr) - 1)

#define SO_ACCEPTCONN 1  // Socket is accepting connections.
#define SO_ERROR 2       // Socket error status.
#define SO_TYPE 3        // Socket type.

#define SOMAXCONN _INT_MAX  // The maximum backlog queue length.

#define MSG_CTRUNC 0x1    // Control data truncated.
#define MSG_EOR 0x2       // Terminates a record.
#define MSG_PEEK 0x4      // Leave received data in queue.
#define MSG_TRUNC 0x8     // Normal data truncated.
#define MSG_WAITALL 0x10  // Attempt to fill the read buffer.

#define MSG_NOSIGNAL 0  // No SIGPIPE will be generated (default).

#define AF_UNSPEC 0  // Unspecified.
#define AF_INET 1    // Internet domain sockets for use with IPv4 addresses.
#define AF_INET6 2   // Internet domain sockets for use with IPv6 addresses.
#define AF_UNIX 3    // UNIX domain sockets.

#define SHUT_RD 0x1                    // Disables further receive operations.
#define SHUT_WR 0x2                    // Disables further send operations.
#define SHUT_RDWR (SHUT_RD | SHUT_WR)  // Disables send and receive operations.

__BEGIN_DECLS
int accept(int, struct sockaddr *__restrict, size_t *__restrict);
int bindat(int, int, const char *);
int connectat(int, int, const char *);
int getpeername(int, struct sockaddr *__restrict, size_t *__restrict);
int getsockname(int, struct sockaddr *__restrict, size_t *__restrict);
int getsockopt(int, int, int, void *__restrict, size_t *__restrict);
int listen(int, int);
ssize_t recv(int, void *, size_t, int);
ssize_t recvfrom(int, void *__restrict, size_t, int,
                 struct sockaddr *__restrict, size_t *__restrict);
ssize_t recvmsg(int, struct msghdr *, int);
ssize_t send(int, const void *, size_t, int);
ssize_t sendmsg(int, const struct msghdr *, int);
int shutdown(int, int);
int socket(int, int, int);
int socketpair(int, int, int, int *);
__END_DECLS

#define recv(socket, buffer, length, flags) \
  recvfrom(socket, buffer, length, flags, _NULL, _NULL)

#endif
