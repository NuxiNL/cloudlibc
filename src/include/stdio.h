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

// <stdio.h> - standard buffered input/output
//
// Extensions:
// - asprintf(), asprintf_l(), vasprintf() and vasprintf_l():
//   Available on all commonly used operating systems.
// - fdopen_l(), fmemopen_l(), open_memstream_l() and tmpfile_l():
//   Sets the character set of the file.
// - fopenat() and fopenat_l():
//   Replacement for fopen().
// - fswap():
//   Toggles the contents of two FILE objects. Can be used as a
//   replacement for freopen().
// - perror_l(), *printf_l() and *scanf_l():
//   perror(), *print() and *scanf() always use the C locale.
//
// Features missing:
// - fopen(), freopen(), rename(), ctermid(), tempnam(), tmpnam(),
//   L_ctermid, L_tmpnam, P_tmpdir and TMP_MAX:
//   Requires global filesystem namespace.
// - getchar(), getchar_unlocked(), gets(), printf(), putchar(),
//   putchar_unlocked(), puts(), scanf(), vprintf(), vscanf(), stdin and
//   stdout:
//   Standard descriptors stdin and stdout are not available.
// - popen() and pclose():
//   Requires a command shell.

#ifndef _STDIO_H_
#define _STDIO_H_

#include <_/limits.h>
#include <_/types.h>

#ifndef _FILE_DECLARED
typedef struct _FILE FILE;
#define _FILE_DECLARED
#endif
#ifndef _OFF_T_DECLARED
typedef __off_t off_t;
#define _OFF_T_DECLARED
#endif
#ifndef _SIZE_T_DECLARED
typedef __size_t size_t;
#define _SIZE_T_DECLARED
#endif
#ifndef _SSIZE_T_DECLARED
typedef __ssize_t ssize_t;
#define _SSIZE_T_DECLARED
#endif
#ifndef _VA_LIST_DECLARED
typedef __va_list va_list;
#define _VA_LIST_DECLARED
#endif

typedef struct {
  off_t __offset;         // Offset within file.
  __mbstate_t __mbstate;  // Multibyte state at this offset.
} fpos_t;

#define BUFSIZ 4096  // Size of <stdio.h> buffers.

#define _IOFBF 1  // Input/output fully buffered.
#define _IOLBF 2  // Input/output line buffered.
#define _IONBF 3  // Input/output unbuffered.

#define SEEK_CUR 1  // Seek relative to current position.
#define SEEK_END 2  // Seek relative to end-of-file.
#define SEEK_SET 3  // Seek relative to start-of-file.

#define FILENAME_MAX 1024   // Recommended filename size.
#define FOPEN_MAX _INT_MAX  // Not limited.

#define EOF (-1)  // End-of-file return value.

#define NULL _NULL

// Standard error output stream.
extern FILE __stderr;
static FILE *const stderr = &__stderr;
#define stderr stderr

__BEGIN_DECLS
int asprintf(char **, const char *, ...) __printflike(2, 3);
int asprintf_l(char **, __locale_t, const char *, ...) __printflike(3, 4);
void clearerr(FILE *);
int dprintf(int, const char *__restrict, ...) __printflike(2, 3);
int dprintf_l(int, __locale_t, const char *__restrict, ...) __printflike(3, 4);
int fclose(FILE *);
FILE *fdopen(int, const char *) __malloc_like;
FILE *fdopen_l(int, const char *, __locale_t) __malloc_like;
int feof(FILE *);
int ferror(FILE *);
int fflush(FILE *);
int fgetc(FILE *);
int fgetpos(FILE *__restrict, fpos_t *__restrict);
char *fgets(char *__restrict, int, FILE *__restrict);
int fileno(FILE *);
void flockfile(FILE *__stream) __locks_exclusive(*__stream);
FILE *fmemopen(void *__restrict, size_t, const char *__restrict) __malloc_like;
FILE *fmemopen_l(void *__restrict, size_t, const char *__restrict,
                 __locale_t) __malloc_like;
FILE *fopenat(int, const char *__restrict,
              const char *__restrict) __malloc_like;
FILE *fopenat_l(int, const char *__restrict, const char *__restrict,
                __locale_t) __malloc_like;
int fprintf(FILE *__restrict, const char *__restrict, ...) __printflike(2, 3);
int fprintf_l(FILE *__restrict, __locale_t, const char *__restrict, ...)
    __printflike(3, 4);
int fputc(int, FILE *);
int fputs(const char *__restrict, FILE *__restrict);
size_t fread(void *__restrict, size_t, size_t, FILE *__restrict);
int fscanf(FILE *__restrict, const char *__restrict, ...) __scanflike(2, 3);
int fscanf_l(FILE *__restrict, __locale_t, const char *__restrict, ...)
    __scanflike(3, 4);
int fseek(FILE *, long, int);
int fseeko(FILE *, off_t, int);
int fsetpos(FILE *, const fpos_t *);
void fswap(FILE *, FILE *);
long ftell(FILE *);
off_t ftello(FILE *);
int ftrylockfile(FILE *__stream) __trylocks_exclusive(0, *__stream);
void funlockfile(FILE *__stream) __unlocks(*__stream);
size_t fwrite(const void *__restrict, size_t, size_t, FILE *__restrict);
int getc(FILE *);
int getc_unlocked(FILE *__stream) __requires_exclusive(*__stream);
ssize_t getdelim(char **__restrict, size_t *__restrict, int, FILE *__restrict);
ssize_t getline(char **__restrict, size_t *__restrict, FILE *__restrict);
FILE *open_memstream(char **, size_t *) __malloc_like;
FILE *open_memstream_l(char **, size_t *, __locale_t) __malloc_like;
void perror(const char *);
void perror_l(const char *, __locale_t);
int putc(int, FILE *);
int putc_unlocked(int, FILE *__stream) __requires_exclusive(*__stream);
int renameat(int, const char *, int, const char *);
void rewind(FILE *);
void setbuf(FILE *__restrict, char *__restrict);
int setvbuf(FILE *__restrict, char *__restrict, int, size_t);
int snprintf(char *__restrict, size_t, const char *__restrict, ...)
    __printflike(3, 4);
int snprintf_l(char *__restrict, size_t, __locale_t, const char *__restrict,
               ...) __printflike(4, 5);
int sscanf(const char *__restrict, const char *__restrict, ...)
    __scanflike(2, 3);
int sscanf_l(const char *__restrict, __locale_t, const char *__restrict, ...)
    __scanflike(3, 4);
FILE *tmpfile(void);
FILE *tmpfile_l(__locale_t);
int ungetc(int, FILE *);
int vasprintf(char **, const char *, va_list) __printflike(2, 0);
int vasprintf_l(char **, __locale_t, const char *, va_list) __printflike(3, 0);
int vdprintf(int, const char *__restrict, va_list) __printflike(2, 0);
int vdprintf_l(int, __locale_t, const char *__restrict, va_list)
    __printflike(3, 0);
int vfprintf(FILE *__restrict, const char *__restrict, va_list)
    __printflike(2, 0);
int vfprintf_l(FILE *__restrict, __locale_t, const char *__restrict, va_list)
    __printflike(3, 0);
int vfscanf(FILE *__restrict, const char *__restrict, va_list)
    __scanflike(2, 0);
int vfscanf_l(FILE *__restrict, __locale_t, const char *__restrict, va_list)
    __scanflike(3, 0);
int vsnprintf(char *__restrict, size_t, const char *__restrict, va_list)
    __scanflike(3, 0);
int vsnprintf_l(char *__restrict, size_t, __locale_t, const char *__restrict,
                va_list) __scanflike(4, 0);
int vsscanf(const char *__restrict, const char *__restrict, va_list)
    __scanflike(2, 0);
int vsscanf_l(const char *__restrict, __locale_t, const char *__restrict,
              va_list) __scanflike(3, 0);
__END_DECLS

#if _CLOUDLIBC_INLINE_FUNCTIONS
static __inline int __fseek(FILE *__stream, long __offset, int __whence) {
  _Static_assert(sizeof(off_t) >= sizeof(long), "Argument would get truncated");
  return fseeko(__stream, __offset, __whence);
}
#define fseek(stream, offset, whence) __fseek(stream, offset, whence)

static __inline void __setbuf(FILE *__restrict __stream,
                              char *__restrict __buf) {
  setvbuf(__stream, __buf, __buf != NULL ? _IOFBF : _IONBF, BUFSIZ);
}
#define setbuf(stream, buf) __setbuf(stream, buf)
#endif

#endif

#if !defined(_STDIO_H_UNSAFE_) && defined(_CLOUDLIBC_UNSAFE_STRING_FUNCTIONS)
#define _STDIO_H_UNSAFE_

__BEGIN_DECLS
int sprintf(char *__restrict, const char *__restrict, ...)
    __extname("__sprintf") __printflike(2, 3);
int vsprintf(char *__restrict, const char *__restrict, va_list)
    __extname("__vsprintf") __printflike(2, 0);
__END_DECLS

#endif
