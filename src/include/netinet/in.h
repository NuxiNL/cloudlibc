// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
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

// <netinet/in.h> - Internet address family
//
// Extensions:
// - INADDR_LOOPBACK:
//   Present on most other systems.
//
// Features missing:
// - struct ipv6_mreq, IPV6_JOIN_GROUP, IPV6_LEAVE_GROUP,
//   IPV6_MULTICAST_HOPS, IPV6_MULTICAST_IF, IPV6_MULTICAST_LOOP,
//   IPV6_UNICAST_HOPS and IPV6_V6ONLY:
//   setsockopt() not supported.

#ifndef _NETINET_IN_H_
#define _NETINET_IN_H_

#include <_/endian.h>
#include <_/limits.h>
#include <_/struct/in_addr.h>  // IWYU pragma: export
#include <_/types.h>

#ifndef _IN_ADDR_T_DECLARED
typedef __in_addr_t in_addr_t;
#define _IN_ADDR_T_DECLARED
#endif
#ifndef _IN_PORT_T_DECLARED
typedef __in_port_t in_port_t;
#define _IN_PORT_T_DECLARED
#endif
#ifndef _SA_FAMILY_T_DECLARED
typedef __sa_family_t sa_family_t;
#define _SA_FAMILY_T_DECLARED
#endif
#ifndef _UINT8_T_DECLARED
typedef __uint8_t uint8_t;
#define _UINT8_T_DECLARED
#endif
#ifndef _UINT32_T_DECLARED
typedef __uint32_t uint32_t;
#define _UINT32_T_DECLARED
#endif

struct sockaddr_in {
  _Alignas(__max_align_t) sa_family_t sin_family;  // AF_INET.
  in_port_t sin_port;                              // Port number.
  struct in_addr sin_addr;                         // IP address.
};

struct in6_addr {
  _Alignas(long) uint8_t s6_addr[16];
};

struct sockaddr_in6 {
  _Alignas(__max_align_t) sa_family_t sin6_family;  // AF_INET6.
  in_port_t sin6_port;                              // Port number.
  uint32_t sin6_flowinfo;                           // Flow information.
  struct in6_addr sin6_addr;                        // IPv6 address.
  uint32_t sin6_scope_id;                           // Scope ID.
};

#define IN6ADDR_ANY_INIT                                                      \
  {                                                                           \
    {                                                                         \
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
          0x00, 0x00, 0x00, 0x00                                              \
    }                                                                         \
  }
#define IN6ADDR_LOOPBACK_INIT                                                 \
  {                                                                           \
    {                                                                         \
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
          0x00, 0x00, 0x00, 0x01                                              \
    }                                                                         \
  }

extern const struct in6_addr in6addr_any;
extern const struct in6_addr in6addr_loopback;

#define IPPROTO_IP 0
#define IPPROTO_ICMP 1
#define IPPROTO_TCP 6
#define IPPROTO_UDP 17
#define IPPROTO_IPV6 41
#define IPPROTO_RAW 255

#define INADDR_ANY _UINT32_C(0x00000000)
#define INADDR_BROADCAST _UINT32_C(0xffffffff)
#define INADDR_LOOPBACK _UINT32_C(0x7f000001)

#define INET_ADDRSTRLEN 16
_Static_assert(INET_ADDRSTRLEN == sizeof("255.255.255.255"),
               "INET_ADDRSTRLEN is defined incorrectly.");
#define INET6_ADDRSTRLEN 46
_Static_assert(INET6_ADDRSTRLEN ==
                   sizeof("ffff:ffff:ffff:ffff:ffff:ffff:255.255.255.255"),
               "INET6_ADDRSTRLEN is defined incorrectly.");

__BEGIN_DECLS
int IN6_IS_ADDR_UNSPECIFIED(const struct in6_addr *);
int IN6_IS_ADDR_LOOPBACK(const struct in6_addr *);
int IN6_IS_ADDR_MULTICAST(const struct in6_addr *);
int IN6_IS_ADDR_LINKLOCAL(const struct in6_addr *);
int IN6_IS_ADDR_SITELOCAL(const struct in6_addr *);
int IN6_IS_ADDR_V4MAPPED(const struct in6_addr *);
int IN6_IS_ADDR_V4COMPAT(const struct in6_addr *);
int IN6_IS_ADDR_MC_NODELOCAL(const struct in6_addr *);
int IN6_IS_ADDR_MC_LINKLOCAL(const struct in6_addr *);
int IN6_IS_ADDR_MC_SITELOCAL(const struct in6_addr *);
int IN6_IS_ADDR_MC_ORGLOCAL(const struct in6_addr *);
int IN6_IS_ADDR_MC_GLOBAL(const struct in6_addr *);
__END_DECLS

#if _CLOUDLIBC_INLINE_FUNCTIONS
static __inline int _IN6_IS_ADDR_UNSPECIFIED(const struct in6_addr *__a) {
  return __a->s6_addr[0] == 0x00 && __a->s6_addr[1] == 0x00 &&
         __a->s6_addr[2] == 0x00 && __a->s6_addr[3] == 0x00 &&
         __a->s6_addr[4] == 0x00 && __a->s6_addr[5] == 0x00 &&
         __a->s6_addr[6] == 0x00 && __a->s6_addr[7] == 0x00 &&
         __a->s6_addr[8] == 0x00 && __a->s6_addr[9] == 0x00 &&
         __a->s6_addr[10] == 0x00 && __a->s6_addr[11] == 0x00 &&
         __a->s6_addr[12] == 0x00 && __a->s6_addr[13] == 0x00 &&
         __a->s6_addr[14] == 0x00 && __a->s6_addr[15] == 0x00;
}
#define IN6_IS_ADDR_UNSPECIFIED(a) _IN6_IS_ADDR_UNSPECIFIED(a)

static __inline int _IN6_IS_ADDR_LOOPBACK(const struct in6_addr *__a) {
  return __a->s6_addr[0] == 0x00 && __a->s6_addr[1] == 0x00 &&
         __a->s6_addr[2] == 0x00 && __a->s6_addr[3] == 0x00 &&
         __a->s6_addr[4] == 0x00 && __a->s6_addr[5] == 0x00 &&
         __a->s6_addr[6] == 0x00 && __a->s6_addr[7] == 0x00 &&
         __a->s6_addr[8] == 0x00 && __a->s6_addr[9] == 0x00 &&
         __a->s6_addr[10] == 0x00 && __a->s6_addr[11] == 0x00 &&
         __a->s6_addr[12] == 0x00 && __a->s6_addr[13] == 0x00 &&
         __a->s6_addr[14] == 0x00 && __a->s6_addr[15] == 0x01;
}
#define IN6_IS_ADDR_LOOPBACK(a) _IN6_IS_ADDR_LOOPBACK(a)

static __inline int _IN6_IS_ADDR_MULTICAST(const struct in6_addr *__a) {
  return __a->s6_addr[0] == 0xff;
}
#define IN6_IS_ADDR_MULTICAST(a) _IN6_IS_ADDR_MULTICAST(a)

static __inline int _IN6_IS_ADDR_LINKLOCAL(const struct in6_addr *__a) {
  return __a->s6_addr[0] == 0xfe && (__a->s6_addr[1] & 0xc0) == 0x80;
}
#define IN6_IS_ADDR_LINKLOCAL(a) _IN6_IS_ADDR_LINKLOCAL(a)

static __inline int _IN6_IS_ADDR_SITELOCAL(const struct in6_addr *__a) {
  return __a->s6_addr[0] == 0xfe && (__a->s6_addr[1] & 0xc0) == 0xc0;
}
#define IN6_IS_ADDR_SITELOCAL(a) _IN6_IS_ADDR_SITELOCAL(a)

static __inline int _IN6_IS_ADDR_V4MAPPED(const struct in6_addr *__a) {
  return __a->s6_addr[0] == 0x00 && __a->s6_addr[1] == 0x00 &&
         __a->s6_addr[2] == 0x00 && __a->s6_addr[3] == 0x00 &&
         __a->s6_addr[4] == 0x00 && __a->s6_addr[5] == 0x00 &&
         __a->s6_addr[6] == 0x00 && __a->s6_addr[7] == 0x00 &&
         __a->s6_addr[8] == 0x00 && __a->s6_addr[9] == 0x00 &&
         __a->s6_addr[10] == 0xff && __a->s6_addr[11] == 0xff;
}
#define IN6_IS_ADDR_V4MAPPED(a) _IN6_IS_ADDR_V4MAPPED(a)

static __inline int _IN6_IS_ADDR_V4COMPAT(const struct in6_addr *__a) {
  return __a->s6_addr[0] == 0x00 && __a->s6_addr[1] == 0x00 &&
         __a->s6_addr[2] == 0x00 && __a->s6_addr[3] == 0x00 &&
         __a->s6_addr[4] == 0x00 && __a->s6_addr[5] == 0x00 &&
         __a->s6_addr[6] == 0x00 && __a->s6_addr[7] == 0x00 &&
         __a->s6_addr[8] == 0x00 && __a->s6_addr[9] == 0x00 &&
         __a->s6_addr[10] == 0x00 && __a->s6_addr[11] == 0x00 &&
         (__a->s6_addr[12] != 0x00 || __a->s6_addr[13] != 0x00 ||
          __a->s6_addr[14] != 0x00 ||
          (__a->s6_addr[15] != 0x00 && __a->s6_addr[15] != 0x01));
}
#define IN6_IS_ADDR_V4COMPAT(a) _IN6_IS_ADDR_V4COMPAT(a)

static __inline int _IN6_IS_ADDR_MC_NODELOCAL(const struct in6_addr *__a) {
  return IN6_IS_ADDR_MULTICAST(__a) && (__a->s6_addr[1] & 0xf) == 0x1;
}
#define IN6_IS_ADDR_MC_NODELOCAL(a) _IN6_IS_ADDR_MC_NODELOCAL(a)

static __inline int _IN6_IS_ADDR_MC_LINKLOCAL(const struct in6_addr *__a) {
  return IN6_IS_ADDR_MULTICAST(__a) && (__a->s6_addr[1] & 0xf) == 0x2;
}
#define IN6_IS_ADDR_MC_LINKLOCAL(a) _IN6_IS_ADDR_MC_LINKLOCAL(a)

static __inline int _IN6_IS_ADDR_MC_SITELOCAL(const struct in6_addr *__a) {
  return IN6_IS_ADDR_MULTICAST(__a) && (__a->s6_addr[1] & 0xf) == 0x5;
}
#define IN6_IS_ADDR_MC_SITELOCAL(a) _IN6_IS_ADDR_MC_SITELOCAL(a)

static __inline int _IN6_IS_ADDR_MC_ORGLOCAL(const struct in6_addr *__a) {
  return IN6_IS_ADDR_MULTICAST(__a) && (__a->s6_addr[1] & 0xf) == 0x8;
}
#define IN6_IS_ADDR_MC_ORGLOCAL(a) _IN6_IS_ADDR_MC_ORGLOCAL(a)

static __inline int _IN6_IS_ADDR_MC_GLOBAL(const struct in6_addr *__a) {
  return IN6_IS_ADDR_MULTICAST(__a) && (__a->s6_addr[1] & 0xf) == 0xe;
}
#define IN6_IS_ADDR_MC_GLOBAL(a) _IN6_IS_ADDR_MC_GLOBAL(a)
#endif

#endif
