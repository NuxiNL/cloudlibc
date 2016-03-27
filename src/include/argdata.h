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

// <argdata.h> - argument data for programs

#ifndef _ARGDATA_H_
#define _ARGDATA_H_

#include <_/limits.h>
#include <_/types.h>

#ifndef _ARGDATA_T_DECLARED
typedef __argdata_t argdata_t;
#define _ARGDATA_T_DECLARED
#endif

typedef struct {
	__size_t index;
	int error;
	__size_t __data[16];
} argdata_iterator_t;

struct _FILE;
struct timespec;

extern const argdata_t argdata_false;
extern const argdata_t argdata_null;
extern const argdata_t argdata_true;

__BEGIN_DECLS
argdata_t *argdata_create_binary(const void *, __size_t);
argdata_t *argdata_create_buffer(const void *, __size_t);
argdata_t *argdata_create_fd(int);
argdata_t *argdata_create_float(double);
argdata_t *__argdata_create_int_s(__intmax_t);
argdata_t *__argdata_create_int_u(__uintmax_t);
argdata_t *argdata_create_map(argdata_t const *const *,
                              argdata_t const *const *, __size_t);
argdata_t *argdata_create_seq(argdata_t const *const *, __size_t);
argdata_t *argdata_create_str(const char *, __size_t);
argdata_t *argdata_create_str_c(const char *);
argdata_t *argdata_create_timestamp(const struct timespec *);
void argdata_free(argdata_t *);
int argdata_get_binary(const argdata_t *, const void **, __size_t *);
void argdata_get_buffer_length(const argdata_t *, __size_t *, __size_t *);
__size_t argdata_get_buffer(const argdata_t *, void *, int *);
int argdata_get_bool(const argdata_t *, _Bool *);
int argdata_get_fd(const argdata_t *, int *);
int argdata_get_float(const argdata_t *, double *);
int __argdata_get_int_s(const argdata_t *, __intmax_t *, __intmax_t,
                        __intmax_t);
int __argdata_get_int_u(const argdata_t *, __uintmax_t *, __uintmax_t);
int argdata_get_str(const argdata_t *, const char **, __size_t *);
int argdata_get_str_c(const argdata_t *, const char **);
int argdata_get_timestamp(const argdata_t *, struct timespec *);
int argdata_map_iterate(const argdata_t *, argdata_iterator_t *);
int argdata_seq_iterate(const argdata_t *, argdata_iterator_t *);
_Bool argdata_map_next(argdata_iterator_t *, const argdata_t **,
                                const argdata_t**);
_Bool argdata_seq_next(argdata_iterator_t *, const argdata_t **);
_Bool argdata_has_next(argdata_iterator_t *);
void argdata_print_yaml(const argdata_t *, struct _FILE *);
__END_DECLS

// Generic fetching of integer values.

#define _ARGDATA_INT_S(type, stype, min, max)                          \
  static __inline int __argdata_get_int_##stype(const argdata_t *__ad, \
                                                type *__value) {       \
    __intmax_t __v;                                                    \
    int __error;                                                       \
                                                                       \
    __error = __argdata_get_int_s(__ad, &__v, min, max);               \
    if (__error != 0)                                                  \
      return __error;                                                  \
    *__value = (type)__v;                                              \
    return 0;                                                          \
  }
#define _ARGDATA_INT_U(type, stype, max)                               \
  static __inline int __argdata_get_int_##stype(const argdata_t *__ad, \
                                                type *__value) {       \
    __uintmax_t __v;                                                   \
    int __error;                                                       \
                                                                       \
    __error = __argdata_get_int_u(__ad, &__v, max);                    \
    if (__error != 0)                                                  \
      return __error;                                                  \
    *__value = (type)__v;                                              \
    return 0;                                                          \
  }
_ARGDATA_INT_S(char, char, _CHAR_MIN, _CHAR_MAX)
_ARGDATA_INT_S(signed char, schar, _SCHAR_MIN, _SCHAR_MAX)
_ARGDATA_INT_U(unsigned char, uchar, _UCHAR_MAX)
_ARGDATA_INT_S(short, short, _SHRT_MIN, _SHRT_MAX)
_ARGDATA_INT_U(unsigned short, ushort, _USHRT_MAX)
_ARGDATA_INT_S(int, int, _INT_MIN, _INT_MAX)
_ARGDATA_INT_U(unsigned int, uint, _UINT_MAX)
_ARGDATA_INT_S(long, long, _LONG_MIN, _LONG_MAX)
_ARGDATA_INT_U(unsigned long, ulong, _ULONG_MAX)
_ARGDATA_INT_S(long long, llong, _LLONG_MIN, _LLONG_MAX)
_ARGDATA_INT_U(unsigned long long, ullong, _ULONG_MAX)
#undef _ARGDATA_INT_S
#undef _ARGDATA_INT_U

// clang-format off
#define argdata_create_int(value)                  \
  _Generic(value,                                  \
           char: __argdata_create_int_s,           \
           signed char: __argdata_create_int_s,    \
           unsigned char: __argdata_create_int_u,  \
           short: __argdata_create_int_s,          \
           unsigned short: __argdata_create_int_u, \
           int: __argdata_create_int_s,            \
           unsigned int: __argdata_create_int_u,   \
           long: __argdata_create_int_s,           \
           unsigned long: __argdata_create_int_u,  \
           long long: __argdata_create_int_s,      \
           unsigned long long: __argdata_create_int_u)(value)
#define argdata_get_int(ad, value)                   \
  _Generic(*(value),                                 \
           char: __argdata_get_int_char,             \
           signed char: __argdata_get_int_schar,     \
           unsigned char: __argdata_get_int_uchar,   \
           short: __argdata_get_int_short,           \
           unsigned short: __argdata_get_int_ushort, \
           int: __argdata_get_int_int,               \
           unsigned int: __argdata_get_int_uint,     \
           long: __argdata_get_int_long,             \
           unsigned long: __argdata_get_int_ulong,   \
           long long: __argdata_get_int_llong,       \
           unsigned long long: __argdata_get_int_ullong)(ad, value)
// clang-format on

#endif
