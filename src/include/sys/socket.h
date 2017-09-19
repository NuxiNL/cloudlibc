// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
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
// - CMSG_LEN() and CMSG_SPACE():
//   Part of RFC 2292 and upcoming versions of POSIX.
// - PF_*:
//   Present on most other systems.
//
// Features missing:
// - SOCK_SEQPACKET and MSG_EOR:
//   Sequential packet sockets are unsupported.
// - SO_* and setsockopt():
//   Socket parameters cannot be adjusted.
// - SOMAXCONN, struct msghdr::msg_name, struct msghdr::msg_namelen,
//   accept(), bind(), connect(), getpeername(), getsockname(), listen(),
//   recvfrom(), sendto() and socket():
//   Only anonymous, addressless sockets are supported.
// - MSG_OOB and sockatmark():
//   Sending and receiving out-of-band data is unsupported.

#ifndef _SYS_SOCKET_H_
#define _SYS_SOCKET_H_

#include <_/limits.h>
#include <_/struct/iovec.h>  // IWYU pragma: export
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
  char sa_data[0];  // Contains nothing meaningful.
};

struct sockaddr_storage {
  _Alignas(__max_align_t) sa_family_t ss_family;
  char __ss_data[32];
};

struct msghdr {
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
  _Alignas(__max_align_t) unsigned char __cmsg_data[];  // Data.
};

#define SOCK_DGRAM 0x80   // Datagram socket.
#define SOCK_STREAM 0x82  // Byte-stream socket.

#define SOL_SOCKET _INT_MAX  // Options to be accessed at socket level.

#define SCM_RIGHTS 1  // File descriptor passing for UNIX sockets.

#define CMSG_DATA(cmsg) (cmsg)->__cmsg_data
#define CMSG_NXTHDR CMSG_NXTHDR
#define CMSG_FIRSTHDR CMSG_FIRSTHDR
#define CMSG_LEN(len) __offsetof(struct cmsghdr, __cmsg_data[len])
#define CMSG_SPACE(len) __roundup(CMSG_LEN(len), _Alignof(struct cmsghdr))

#define SO_TYPE 3  // Socket type.

// Input flags for recvmsg().
#define MSG_PEEK 0x4      // Leave received data in queue.
#define MSG_WAITALL 0x10  // Attempt to fill the read buffer.

// Output flags for recvmsg().
#define MSG_CTRUNC 0x1  // Control data truncated.
#define MSG_TRUNC 0x8   // Normal data truncated.

#define MSG_NOSIGNAL 0  // No SIGPIPE will be generated (default).

#define AF_UNSPEC 0  // Unspecified.
#define AF_INET 1    // Internet domain sockets for use with IPv4 addresses.
#define AF_INET6 2   // Internet domain sockets for use with IPv6 addresses.
#define AF_UNIX 3    // UNIX domain sockets.

// For compatibility.
#define PF_UNSPEC AF_UNSPEC
#define PF_INET AF_INET
#define PF_INET6 AF_INET6
#define PF_UNIX AF_UNIX

#define SHUT_RD 0x1                    // Disables further receive operations.
#define SHUT_WR 0x2                    // Disables further send operations.
#define SHUT_RDWR (SHUT_RD | SHUT_WR)  // Disables send and receive operations.

__BEGIN_DECLS
struct cmsghdr *CMSG_FIRSTHDR(const struct msghdr *);
struct cmsghdr *CMSG_NXTHDR(const struct msghdr *, const struct cmsghdr *);
int getsockopt(int, int, int, void *__restrict, size_t *__restrict);
ssize_t recv(int, void *, size_t, int);
ssize_t recvmsg(int, struct msghdr *, int);
ssize_t send(int, const void *, size_t, int);
ssize_t sendmsg(int, const struct msghdr *, int);
int shutdown(int, int);
int socketpair(int, int, int, int *);
__END_DECLS

#endif
