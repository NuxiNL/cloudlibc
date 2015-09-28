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

#ifndef ___ENDIAN_H_
#define ___ENDIAN_H_

#include <_/types.h>

__BEGIN_DECLS
__uint32_t htonl(__uint32_t) __pure2;
__uint16_t htons(__uint16_t) __pure2;
__uint32_t ntohl(__uint32_t) __pure2;
__uint16_t ntohs(__uint16_t) __pure2;
__END_DECLS

#if _CLOUDLIBC_INLINE_FUNCTIONS
static __inline __uint32_t __htonl(__uint32_t __i) {
  union {
    __uint8_t __host[4];
    __uint32_t __net;
  } __v = {.__host = {(__uint8_t)(__i >> 24), (__uint8_t)(__i >> 16),
                      (__uint8_t)(__i >> 8), (__uint8_t)__i}};
  return __v.__net;
}
#define htonl(i) __htonl(i)

static __inline __uint16_t __htons(__uint16_t __i) {
  union {
    __uint8_t __host[2];
    __uint16_t __net;
  } __v = {.__host = {(__uint8_t)(__i >> 8), (__uint8_t)__i}};
  return __v.__net;
}
#define htons(i) __htons(i)

static __inline __uint32_t __ntohl(__uint32_t __i) {
  union {
    __uint32_t __net;
    __uint8_t __host[4];
  } __v = {.__net = __i};
  return (__uint32_t)((__uint32_t)__v.__host[0] << 24 |
                      (__uint32_t)__v.__host[1] << 16 |
                      (__uint32_t)__v.__host[2] << 8 | __v.__host[3]);
}
#define ntohl(i) __ntohl(i)

static __inline __uint16_t __ntohs(__uint16_t __i) {
  union {
    __uint16_t __net;
    __uint8_t __host[2];
  } __v = {.__net = __i};
  return (__uint16_t)((__uint16_t)__v.__host[0] << 8 | __v.__host[1]);
}
#define ntohs(i) __ntohs(i)
#endif

#endif
