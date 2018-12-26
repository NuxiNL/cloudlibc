// Copyright (c) 2018 Nuxi, https://nuxi.nl/
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

// <cloudlibc_interceptors.h> - non-aliased versions of interceptable functions
//
// This header defines the original cloudlibc functions, so software like the
// Address Sanitizer is able to access them after intercepting base functions.
// For example, it defines __cloudlibc_strlen, which provides the actual
// implementation for strlen. If the strlen function is intercepted, it can
// call the original function using the name __cloudlibc_strlen.

#ifndef _CLOUDLIBC_INTERCEPTORS_H_
#define _CLOUDLIBC_INTERCEPTORS_H_

#include <dirent.h>
#include <inttypes.h>
#include <netdb.h>
#include <poll.h>
#include <pthread.h>
#include <setjmp.h>
#include <stdio.h>
#include <sys/times.h>
#include <sys/uio.h>
#include <wchar.h>

__BEGIN_DECLS

// dirent.h
struct dirent *__cloudlibc_readdir(DIR *);

// execinfo.h
__size_t __cloudlibc_backtrace(void **, __size_t);
char **__cloudlibc_backtrace_symbols(void *const *, __size_t);

// inttypes.h
intmax_t __cloudlibc_strtoimax(const char *__restrict, char **__restrict, int);
intmax_t __cloudlibc_strtoimax_l(const char *__restrict, char **__restrict, int,
                     __locale_t);
uintmax_t __cloudlibc_strtoumax(const char *__restrict, char **__restrict, int);

// locale.h
char *__cloudlibc_setlocale(int, const char *);

// math.h
double __cloudlibc_frexp(double, int *);
float __cloudlibc_frexpf(float, int *);
long double __cloudlibc_frexpl(long double, int *);
double __cloudlibc_lgamma(double);
float __cloudlibc_lgammaf(float);
long double __cloudlibc_lgammal(long double);
double __cloudlibc_remquo(double, double, int *);
float __cloudlibc_remquof(float, float, int *);
long double __cloudlibc_remquol(long double, long double, int *);

// netdb.h
int __cloudlibc_getaddrinfo(const char *__restrict, const char *__restrict,
                const struct addrinfo *__restrict,
                struct addrinfo **__restrict);
int __cloudlibc_getnameinfo(const struct sockaddr *__restrict, __size_t, char *__restrict,
                __size_t, char *__restrict, __size_t, int);

// poll.h
int __cloudlibc_poll(struct pollfd *, __size_t, int);

// pthread.h
int __cloudlibc_pthread_attr_getdetachstate(const pthread_attr_t *, int *);
int __cloudlibc_pthread_attr_getstacksize(const pthread_attr_t *__restrict,
                              size_t *__restrict);
int __cloudlibc_pthread_condattr_getpshared(const pthread_condattr_t *__restrict,
                                int *__restrict);
int __cloudlibc_pthread_mutex_lock(pthread_mutex_t *__mutex) __locks_exclusive(*__mutex);
int __cloudlibc_pthread_mutex_unlock(pthread_mutex_t *__mutex) __unlocks(*__mutex);
int __cloudlibc_pthread_mutexattr_getpshared(const pthread_mutexattr_t *__restrict,
                                 int *__restrict);
int __cloudlibc_pthread_mutexattr_gettype(const pthread_mutexattr_t *__restrict,
                              int *__restrict);
int __cloudlibc_pthread_rwlock_unlock(pthread_rwlock_t *__rwlock) __unlocks(*__rwlock);
int __cloudlibc_pthread_rwlock_wrlock(pthread_rwlock_t *__rwlock)
    __locks_exclusive(*__rwlock);
int __cloudlibc_pthread_rwlockattr_getpshared(const pthread_rwlockattr_t *__restrict,
                                  int *__restrict);
int __cloudlibc_pthread_spin_lock(pthread_spinlock_t *__lock) __locks_exclusive(*__lock);
int __cloudlibc_pthread_spin_unlock(pthread_spinlock_t *__lock) __unlocks(*__lock);
int __cloudlibc_pthread_create(pthread_t *__restrict, const pthread_attr_t *__restrict,
                   void *(*)(void *), void *__restrict);
int __cloudlibc_pthread_join(pthread_t, void **);

// setjmp.h
_Noreturn void __cloudlibc_longjmp(jmp_buf, int);
_Noreturn void __cloudlibc_siglongjmp(sigjmp_buf, int);

// stdio.h
int __cloudlibc_asprintf(char **, const char *, ...) __printflike(2, 3);
int __cloudlibc_fclose(FILE *);
FILE *__cloudlibc_fdopen(int, const char *) __malloc_like;
int __cloudlibc_fflush(FILE *);
int __cloudlibc_fprintf(FILE *__restrict, const char *__restrict, ...) __printflike(2, 3);
size_t __cloudlibc_fread(void *__restrict, size_t, size_t, FILE *__restrict);
int __cloudlibc_fscanf(FILE *__restrict, const char *__restrict, ...) __scanflike(2, 3);
size_t __cloudlibc_fwrite(const void *__restrict, size_t, size_t, FILE *__restrict);
int __cloudlibc_snprintf(char *__restrict, size_t, const char *__restrict, ...)
    __printflike(3, 4);
int __cloudlibc_sprintf(char *__restrict, const char *__restrict, ...)
    __printflike(2, 3);
int __cloudlibc_vsprintf(char *__restrict, const char *__restrict, va_list)
    __printflike(2, 0);
int __cloudlibc_sscanf(const char *__restrict, const char *__restrict, ...)
    __scanflike(2, 3);
int __cloudlibc_vasprintf(char **, const char *, va_list) __printflike(2, 0);
int __cloudlibc_vfprintf(FILE *__restrict, const char *__restrict, va_list)
    __printflike(2, 0);
int __cloudlibc_vfscanf(FILE *__restrict, const char *__restrict, va_list)
    __scanflike(2, 0);
int __cloudlibc_vsnprintf(char *__restrict, size_t, const char *__restrict, va_list)
    __scanflike(3, 0);
int __cloudlibc_vsscanf(const char *__restrict, const char *__restrict, va_list)
    __scanflike(2, 0);
void *__cloudlibc_malloc(size_t) __malloc_like;
void *__cloudlibc_calloc(size_t, size_t) __malloc_like;
int __cloudlibc_posix_memalign(void **, size_t, size_t);
void *__cloudlibc_aligned_alloc(size_t, size_t) __malloc_like;
void *__cloudlibc_realloc(void *, size_t) __malloc_like;
void __cloudlibc_free(void*);

// stdlib.h
int __cloudlibc_atoi(const char *);
long __cloudlibc_atol(const char *);
long long __cloudlibc_atoll(const char *);
size_t __cloudlibc_mbstowcs(wchar_t *__restrict, const char *__restrict, size_t);
long __cloudlibc_strtol(const char *__restrict, char **__restrict, int);
long long __cloudlibc_strtoll(const char *__restrict, char **__restrict, int);
size_t __cloudlibc_wcstombs(char *__restrict, const wchar_t *__restrict, size_t);
int __cloudlibc___cxa_atexit(void (*)(void *), void *, void *);

// string.h
void *__cloudlibc_memchr(const void *, int, size_t) __pure;
int __cloudlibc_memcmp(const void *, const void *, size_t) __pure;
void *__cloudlibc_memcpy(void *__restrict, const void *__restrict, size_t);
void *__cloudlibc_memmem(const void *, size_t, const void *, size_t) __pure;
void *__cloudlibc_memmove(void *, const void *, size_t);
void *__cloudlibc_memrchr(const void *, int, size_t) __pure;
void *__cloudlibc_memset(void *, int, size_t);
char *__cloudlibc_strcat(char *__restrict, const char *__restrict);
char *__cloudlibc_strchr(const char *, int) __pure;
char *__cloudlibc_strcpy(char *__restrict, const char *__restrict);
size_t __cloudlibc_strcspn(const char *, const char *) __pure;
char *__cloudlibc_strdup(const char *) __malloc_like;
int __cloudlibc_strerror_r(int, char *, size_t);
char *__cloudlibc_strerror(int) __pure;
size_t __cloudlibc_strlen(const char *) __pure;
char *__cloudlibc_strncat(char *__restrict, const char *__restrict, size_t);
char *__cloudlibc_strncpy(char *__restrict, const char *__restrict, size_t);
char *__cloudlibc_strndup(const char *, size_t) __malloc_like;
char *__cloudlibc_strpbrk(const char *, const char *) __pure;
char *__cloudlibc_strrchr(const char *, int) __pure;
size_t __cloudlibc_strspn(const char *, const char *) __pure;
char *__cloudlibc_strstr(const char *, const char *) __pure;
size_t __cloudlibc_strnlen(const char *, size_t) __pure;
int __cloudlibc_strncmp(const char *, const char *, size_t) __pure;
int __cloudlibc_strcmp(const char *, const char *) __pure;

// strings.h
int __cloudlibc_strcasecmp(const char *, const char *) __pure;
int __cloudlibc_strncasecmp(const char *, const char *, size_t) __pure;

// time.h
int __cloudlibc_clock_getres(clockid_t, struct timespec *);
int __cloudlibc_clock_gettime(clockid_t, struct timespec *);

// unistd.h
ssize_t __cloudlibc_pread(int, void *, size_t, off_t);
ssize_t __cloudlibc_pwrite(int, const void *, size_t, off_t);
ssize_t __cloudlibc_read(int, void *, size_t);
ssize_t __cloudlibc_write(int, const void *, size_t);

// wchar.h
size_t __cloudlibc_mbsnrtowcs(wchar_t *__restrict, const char **__restrict, size_t, size_t,
                  mbstate_t *__restrict);
size_t __cloudlibc_mbsrtowcs(wchar_t *__restrict, const char **__restrict, size_t,
                 mbstate_t *__restrict);
size_t __cloudlibc_wcrtomb(char *__restrict, wchar_t, mbstate_t *__restrict);
wchar_t *__cloudlibc_wcscat(wchar_t *__restrict, const wchar_t *__restrict);
size_t __cloudlibc_wcslen(const wchar_t *) __pure;
wchar_t *__cloudlibc_wcsncat(wchar_t *__restrict, const wchar_t *__restrict, size_t);
size_t __cloudlibc_wcsnlen(const wchar_t *, size_t) __pure;
size_t __cloudlibc_wcsnrtombs(char *__restrict, const wchar_t **__restrict, size_t, size_t,
                  mbstate_t *__restrict);
size_t __cloudlibc_wcsrtombs(char *__restrict, const wchar_t **__restrict, size_t,
                 mbstate_t *__restrict);

// sys/ioctl.h
int __cloudlibc_ioctl(int, int, ...);

// sys/socket.h
int __cloudlibc_getsockopt(int, int, int, void *__restrict, size_t *__restrict);

// sys/times.h
clock_t __cloudlibc_times(struct tms *);

// sys/uio.h
ssize_t __cloudlibc_preadv(int, const struct iovec *, int, __off_t);
ssize_t __cloudlibc_pwritev(int, const struct iovec *, int, __off_t);
ssize_t __cloudlibc_readv(int, const struct iovec *, int);
ssize_t __cloudlibc_writev(int, const struct iovec *, int);

__END_DECLS

#endif
