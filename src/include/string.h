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

// <string.h> - string operations
//
// Extensions:
// - memmem(), memrchr(), strlcat() and strlcpy(), strsep():
//   Available on many commonly used operating systems.
// - strsignal_l():
//   strsignal() always uses the C locale.
// - strverscmp():
//   Can be used to compare strings with numbers in them. Also present
//   on Linux.
//
// Features missing:
// - strtok():
//   Not thread-safe.

#ifndef _STRING_H_
#define _STRING_H_

#include <_/types.h>

#define NULL _NULL

#ifndef _LOCALE_T_DECLARED
typedef __locale_t locale_t;
#define _LOCALE_T_DECLARED
#endif
#ifndef _SIZE_T_DECLARED
typedef __size_t size_t;
#define _SIZE_T_DECLARED
#endif

__BEGIN_DECLS
void *memccpy(void *__restrict, const void *__restrict, int, size_t);
void *memchr(const void *, int, size_t) __pure;
int memcmp(const void *, const void *, size_t) __pure;
void *memcpy(void *__restrict, const void *__restrict, size_t);
void *memmem(const void *, size_t, const void *, size_t) __pure;
void *memmove(void *, const void *, size_t);
void *memrchr(const void *, int, size_t) __pure;
void *memset(void *, int, size_t);
char *stpncpy(char *__restrict, const char *__restrict, size_t);
char *strchr(const char *, int) __pure;
int strcmp(const char *, const char *) __pure;
int strcoll(const char *, const char *) __pure;
int strcoll_l(const char *, const char *, locale_t) __pure;
size_t strcspn(const char *, const char *) __pure;
char *strdup(const char *) __malloc_like;
char *strerror(int) __pure;
char *strerror_l(int, locale_t) __pure;
int strerror_r(int, char *, size_t);
size_t strlcat(char *__restrict, const char *__restrict, size_t);
size_t strlcpy(char *__restrict, const char *__restrict, size_t);
size_t strlen(const char *) __pure;
int strncmp(const char *, const char *, size_t) __pure;
char *strncpy(char *__restrict, const char *__restrict, size_t);
char *strndup(const char *, size_t) __malloc_like;
size_t strnlen(const char *, size_t) __pure;
char *strpbrk(const char *, const char *) __pure;
char *strrchr(const char *, int) __pure;
char *strsep(char **, const char *);
char *strsignal(int) __pure;
char *strsignal_l(int, locale_t) __pure;
size_t strspn(const char *, const char *) __pure;
char *strstr(const char *, const char *) __pure;
char *strtok_r(char *__restrict, const char *__restrict, char **__restrict);
int strverscmp(const char *, const char *) __pure;
size_t strxfrm(char *__restrict, const char *__restrict, size_t);
size_t strxfrm_l(char *__restrict, const char *__restrict, size_t, locale_t);
__END_DECLS

#if _CLOUDLIBC_INLINE_FUNCTIONS
#define memchr(s, c, n) __preserve_const(void, memchr, s, s, c, n)
#define memmem(s1, s1len, s2, s2len) \
  __preserve_const(void, memmem, s1, s1, s1len, s2, s2len)
#define memrchr(s, c, n) __preserve_const(void, memrchr, s, s, c, n)
#define strchr(s, n) __preserve_const(char, strchr, s, s, n)
#define strpbrk(s1, s2) __preserve_const(char, strpbrk, s1, s1, s2)
#define strrchr(s, n) __preserve_const(char, strrchr, s, s, n)
#define strstr(s1, s2) __preserve_const(char, strstr, s1, s1, s2)
#endif

#endif

#if !defined(_STRING_H_UNSAFE_) && defined(_CLOUDLIBC_UNSAFE_STRING_FUNCTIONS)
#define _STRING_H_UNSAFE_

__BEGIN_DECLS
char *stpcpy(char *__restrict, const char *__restrict) __extname("__stpcpy");
char *strcat(char *__restrict, const char *__restrict) __extname("__strcat");
char *strcpy(char *__restrict, const char *__restrict) __extname("__strcpy");
char *strncat(char *__restrict, const char *__restrict, size_t)
    __extname("__strncat");
__END_DECLS

#endif
