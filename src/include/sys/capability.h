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

// <sys/capability.h> - file descriptor access controls

#ifndef _SYS_CAPABILITY_H_
#define _SYS_CAPABILITY_H_

#include <_/limits.h>
#include <_/types.h>

typedef __uint64_t __cap_rights_bits_t;
typedef struct { __cap_rights_bits_t __value; } cap_rights_t;

#define _CAP_BIT(f) (_UINT64_C(1) << (f))
#define _CAP_SENTINEL _UINT64_C(0)

// Primitives.
#define CAP_FEXECVE _CAP_BIT(0)                    // exec().
#define CAP_FPATHCONF _CAP_BIT(1)                  // fpathconf().
#define CAP_FSTAT _CAP_BIT(2)                      // fstat().
#define CAP_FSYNC _CAP_BIT(3)                      // fsync() and fdatasync().
#define CAP_FTRUNCATE _CAP_BIT(4)                  // ftruncate().
#define CAP_FUTIMES _CAP_BIT(5)                    // futimens().
#define CAP_LINKAT (_CAP_BIT(6) | CAP_LOOKUP)      // linkat().
#define CAP_LOOKUP _CAP_BIT(7)                     // *at().
#define CAP_MKDIRAT (_CAP_BIT(8) | CAP_LOOKUP)     // mkdirat().
#define CAP_MKFIFOAT (_CAP_BIT(9) | CAP_LOOKUP)    // mkfifoat().
#define CAP_MMAP _CAP_BIT(10)                      // mmap(PROT_NONE).
#define CAP_READ _CAP_BIT(11)                      // read().
#define CAP_READDIR _CAP_BIT(12)                   // readdir().
#define CAP_RENAMEAT (_CAP_BIT(13) | CAP_LOOKUP)   // renameat().
#define CAP_SEEK (_CAP_BIT(14) | CAP_SEEK_TELL)    // lseek().
#define CAP_SEEK_TELL _CAP_BIT(15)                 // lseek(0, SEEK_CUR).
#define CAP_SYMLINKAT (_CAP_BIT(16) | CAP_LOOKUP)  // symlinkat().
#define CAP_UNLINKAT (_CAP_BIT(17) | CAP_LOOKUP)   // unlinkat()/renameat().
#define CAP_WRITE _CAP_BIT(18)                     // write() with O_APPEND.

// Values derived from the above.
#define CAP_MMAP_R (CAP_READ | CAP_MMAP | CAP_SEEK)   // mmap(PROT_READ).
#define CAP_MMAP_W (CAP_WRITE | CAP_MMAP | CAP_SEEK)  // mmap(PROT_WRITE).
#define CAP_MMAP_X CAP_MMAP_R                         // mmap(PROT_EXEC).
#define CAP_MMAP_RW (CAP_MMAP_R | CAP_MMAP_W)
#define CAP_MMAP_RX (CAP_MMAP_R | CAP_MMAP_X)
#define CAP_MMAP_WX (CAP_MMAP_W | CAP_MMAP_X)
#define CAP_MMAP_RWX (CAP_MMAP_R | CAP_MMAP_W | CAP_MMAP_X)
#define CAP_PREAD (CAP_READ | CAP_SEEK)  // aio_read(), pread(), etc.
#define CAP_PWRITE \
  (CAP_WRITE |     \
   CAP_SEEK)  // aio_write(), pwrite(), and write() without O_APPEND.
#define CAP_RECV CAP_READ   // recv(), recvfrom() and recvmsg().
#define CAP_SEND CAP_WRITE  // send(), sendto() and sendmsg().

#define cap_rights_clear(rights, ...) \
  __cap_rights_clear(rights, __VA_ARGS__, _CAP_SENTINEL)
#define cap_rights_init(rights, ...) \
  __cap_rights_init(rights, __VA_ARGS__, _CAP_SENTINEL)
#define cap_rights_is_set(rights, ...) \
  __cap_rights_is_set(rights, __VA_ARGS__, _CAP_SENTINEL)
#define cap_rights_set(rights, ...) \
  __cap_rights_set(rights, __VA_ARGS__, _CAP_SENTINEL)

__BEGIN_DECLS
cap_rights_t *__cap_rights_clear(cap_rights_t *, ...);
cap_rights_t *__cap_rights_init(cap_rights_t *, ...);
_Bool __cap_rights_is_set(const cap_rights_t *, ...);
cap_rights_t *__cap_rights_set(cap_rights_t *, ...);
_Bool cap_rights_contains(const cap_rights_t *, const cap_rights_t *);
_Bool cap_rights_equals(const cap_rights_t *, const cap_rights_t *);
int cap_rights_get(int, cap_rights_t *);
int cap_rights_get_explicit(int, cap_rights_t *, cap_rights_t *);
cap_rights_t *cap_rights_intersect(cap_rights_t *, const cap_rights_t *);
int cap_rights_limit(int, const cap_rights_t *);
int cap_rights_limit_explicit(int, const cap_rights_t *, const cap_rights_t *);
cap_rights_t *cap_rights_merge(cap_rights_t *, const cap_rights_t *);
cap_rights_t *cap_rights_remove(cap_rights_t *, const cap_rights_t *);
__END_DECLS

#endif
