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

// <regex.h> - regular expression matching types
//
// Extensions:
// - REG_FNMATCH*:
//   Can be used to match strings like fnmatch().
// - reg*_l():
//   regcomp() and regexec() always uses the C locale.
// - regn*():
//   Allows for regex compilation of or matching on strings without null
//   termination. Also present on Mac OS X.
// - regw*():
//   Allows for regex compilation of or matching on wide characters. Also
//   present on Mac OS X.

#ifndef REGEX_H
#define REGEX_H

#include <_/types.h>

#ifndef _SIZE_T_DECLARED
typedef __size_t size_t;
#define _SIZE_T_DECLARED
#endif
typedef __ssize_t regoff_t;

typedef struct {
  struct __regex *__regex;
  size_t re_nsub;
} regex_t;

typedef struct {
  regoff_t rm_so;
  regoff_t rm_eo;
} regmatch_t;

#define REG_EXTENDED 0x1
#define REG_ICASE 0x2
#define REG_NOSUB 0x4
#define REG_NEWLINE 0x8

#define REG_FNMATCH 0x10
#define REG_FNMATCH_PATHNAME 0x20
#define REG_FNMATCH_PERIOD 0x40
#define REG_FNMATCH_NOESCAPE 0x80

#define REG_NOTBOL 0x1
#define REG_NOTEOL 0x2

#define REG_BADBR 1
#define REG_BADPAT 2
#define REG_BADRPT 3
#define REG_EBRACE 4
#define REG_EBRACK 5
#define REG_ECOLLATE 6
#define REG_ECTYPE 7
#define REG_EESCAPE 8
#define REG_EPAREN 9
#define REG_ERANGE 10
#define REG_ESPACE 11
#define REG_ESUBREG 12
#define REG_NOMATCH 13

__BEGIN_DECLS
int regcomp(regex_t *__restrict, const char *__restrict, int);
int regcomp_l(regex_t *__restrict, const char *__restrict, int, __locale_t);
size_t regerror(int, const regex_t *__restrict, char *__restrict, size_t);
size_t regerror_l(int, const regex_t *__restrict, char *__restrict, size_t,
                  __locale_t);
int regexec(const regex_t *__restrict, const char *__restrict, size_t,
            regmatch_t *__restrict, int);
int regexec_l(const regex_t *__restrict, const char *__restrict, size_t,
              regmatch_t *__restrict, int, __locale_t);
void regfree(regex_t *);
int regncomp(regex_t *__restrict, const char *__restrict, size_t, int);
int regncomp_l(regex_t *__restrict, const char *__restrict, size_t, int,
               __locale_t);
int regnexec(const regex_t *__restrict, const char *__restrict, size_t, size_t,
             regmatch_t *__restrict, int);
int regnexec_l(const regex_t *__restrict, const char *__restrict, size_t,
               size_t, regmatch_t *__restrict, int, __locale_t);
int regwcomp(regex_t *__restrict, const __wchar_t *__restrict, int);
int regwcomp_l(regex_t *__restrict, const __wchar_t *__restrict, int,
               __locale_t);
int regwexec(const regex_t *__restrict, const __wchar_t *__restrict, size_t,
             regmatch_t *__restrict, int);
int regwncomp(regex_t *__restrict, const __wchar_t *__restrict, size_t, int);
int regwncomp_l(regex_t *__restrict, const __wchar_t *__restrict, size_t, int,
                __locale_t);
int regwnexec(const regex_t *__restrict, const __wchar_t *__restrict, size_t,
              size_t, regmatch_t *__restrict, int);
__END_DECLS

#endif
