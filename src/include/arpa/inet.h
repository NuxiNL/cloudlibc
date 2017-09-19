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

// <arpa/inet.h> - definitions for internet operations
//
// Extensions:
// - inet_aton():
//   Present on most operating systems.
//
// Features missing:
// - inet_addr():
//   API does not allow 255.255.255.255 to be parsed. Use inet_aton()
//   instead.
// - inet_ntoa():
//   Not thread-safe. Use inet_ntop() instead.

#ifndef _ARPA_INET_H_
#define _ARPA_INET_H_

#include <_/endian.h>
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
#ifndef _UINT16_T_DECLARED
typedef __uint16_t uint16_t;
#define _UINT16_T_DECLARED
#endif
#ifndef _UINT32_T_DECLARED
typedef __uint32_t uint32_t;
#define _UINT32_T_DECLARED
#endif

#define INET_ADDRSTRLEN 16
#define INET6_ADDRSTRLEN 46

__BEGIN_DECLS
int inet_aton(const char *, struct in_addr *);
const char *inet_ntop(int, const void *__restrict, char *__restrict, __size_t);
int inet_pton(int, const char *__restrict, void *__restrict);
__END_DECLS

#endif
