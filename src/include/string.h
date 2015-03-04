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

// <string.h> - string operations
//
// Extensions:
// - memmem(), memrchr(), strlcat() and strlcpy(), strsep():
//   Available on many commonly used operating systems.
// - strsignal_l():
//   strsignal() always uses the POSIX locale.
//
// Features missing:
// - stpcpy(), strcat(), strcpy() and strncat():
//   Prone to buffer overflows. Not safe to use.
// - strtok():
//   Not thread-safe.

#ifndef _STRING_H_
#define _STRING_H_

#include <_/types.h>

#define NULL _NULL

typedef __locale_t locale_t;
typedef __size_t size_t;

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
size_t strxfrm(char *__restrict, const char *__restrict, size_t);
size_t strxfrm_l(char *__restrict, const char *__restrict, size_t, locale_t);
__END_DECLS

#endif
