// Copyright (c) 2015-2017 Nuxi (https://nuxi.nl/) and contributors.
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

#ifndef ARGDATA_H
#define ARGDATA_H

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#ifndef argdata_t
typedef struct argdata_t argdata_t;
#define argdata_t argdata_t
#endif

typedef struct {
  _Alignas(long) int error;
  char data[128];
} argdata_map_iterator_t;

typedef struct {
  _Alignas(long) int error;
  char data[128];
} argdata_seq_iterator_t;

struct timespec;

extern const argdata_t argdata_false;
extern const argdata_t argdata_null;
extern const argdata_t argdata_true;

#ifdef __cplusplus
extern "C" {
#endif
argdata_t *argdata_create_binary(const void *, size_t);
argdata_t *argdata_create_buffer(const void *, size_t);
argdata_t *argdata_create_fd(int);
argdata_t *argdata_create_float(double);
argdata_t *argdata_create_int_s(intmax_t);
argdata_t *argdata_create_int_u(uintmax_t);
argdata_t *argdata_create_map(argdata_t const *const *,
                              argdata_t const *const *, size_t);
argdata_t *argdata_create_seq(argdata_t const *const *, size_t);
argdata_t *argdata_create_str(const char *, size_t);
argdata_t *argdata_create_str_c(const char *);
argdata_t *argdata_create_timestamp(const struct timespec *);
void argdata_free(argdata_t *);
int argdata_get_binary(const argdata_t *, const void **, size_t *);
void argdata_get_buffer_length(const argdata_t *, size_t *, size_t *);
size_t argdata_get_buffer(const argdata_t *, void *, int *);
int argdata_get_bool(const argdata_t *, bool *);
int argdata_get_fd(const argdata_t *, int *);
int argdata_get_float(const argdata_t *, double *);
int argdata_get_int_s(const argdata_t *, intmax_t *, intmax_t, intmax_t);
int argdata_get_int_u(const argdata_t *, uintmax_t *, uintmax_t);
int argdata_get_str(const argdata_t *, const char **, size_t *);
int argdata_get_str_c(const argdata_t *, const char **);
int argdata_get_timestamp(const argdata_t *, struct timespec *);
int argdata_map_iterate(const argdata_t *, argdata_map_iterator_t *);
bool argdata_map_next(argdata_map_iterator_t *, const argdata_t **,
                      const argdata_t **);
void argdata_print_yaml(const argdata_t *, FILE *);
int argdata_seq_iterate(const argdata_t *, argdata_seq_iterator_t *);
bool argdata_seq_next(argdata_seq_iterator_t *, const argdata_t **);
#ifdef __cplusplus
}
#endif

// Generic fetching of integer values.

#define ARGDATA_INT_S(type, stype, min, max)                     \
  static inline int argdata_get_int_##stype(const argdata_t *ad, \
                                            type *value) {       \
    intmax_t v;                                                  \
    int error;                                                   \
                                                                 \
    error = argdata_get_int_s(ad, &v, min, max);                 \
    if (error != 0)                                              \
      return error;                                              \
    *value = (type)v;                                            \
    return 0;                                                    \
  }
#define ARGDATA_INT_U(type, stype, max)                          \
  static inline int argdata_get_int_##stype(const argdata_t *ad, \
                                            type *value) {       \
    uintmax_t v;                                                 \
    int error;                                                   \
                                                                 \
    error = argdata_get_int_u(ad, &v, max);                      \
    if (error != 0)                                              \
      return error;                                              \
    *value = (type)v;                                            \
    return 0;                                                    \
  }
ARGDATA_INT_S(char, char, CHAR_MIN, CHAR_MAX)
ARGDATA_INT_S(signed char, schar, SCHAR_MIN, SCHAR_MAX)
ARGDATA_INT_U(unsigned char, uchar, UCHAR_MAX)
ARGDATA_INT_S(short, short, SHRT_MIN, SHRT_MAX)
ARGDATA_INT_U(unsigned short, ushort, USHRT_MAX)
ARGDATA_INT_S(int, int, INT_MIN, INT_MAX)
ARGDATA_INT_U(unsigned int, uint, UINT_MAX)
ARGDATA_INT_S(long, long, LONG_MIN, LONG_MAX)
ARGDATA_INT_U(unsigned long, ulong, ULONG_MAX)
ARGDATA_INT_S(long long, llong, LLONG_MIN, LLONG_MAX)
ARGDATA_INT_U(unsigned long long, ullong, ULLONG_MAX)
#undef ARGDATA_INT_S
#undef ARGDATA_INT_U

// clang-format off
#define argdata_create_int(value)                \
  _Generic(value,                                \
           char: argdata_create_int_s,           \
           signed char: argdata_create_int_s,    \
           unsigned char: argdata_create_int_u,  \
           short: argdata_create_int_s,          \
           unsigned short: argdata_create_int_u, \
           int: argdata_create_int_s,            \
           unsigned int: argdata_create_int_u,   \
           long: argdata_create_int_s,           \
           unsigned long: argdata_create_int_u,  \
           long long: argdata_create_int_s,      \
           unsigned long long: argdata_create_int_u)(value)
#define argdata_get_int(ad, value)                 \
  _Generic(*(value),                               \
           char: argdata_get_int_char,             \
           signed char: argdata_get_int_schar,     \
           unsigned char: argdata_get_int_uchar,   \
           short: argdata_get_int_short,           \
           unsigned short: argdata_get_int_ushort, \
           int: argdata_get_int_int,               \
           unsigned int: argdata_get_int_uint,     \
           long: argdata_get_int_long,             \
           unsigned long: argdata_get_int_ulong,   \
           long long: argdata_get_int_llong,       \
           unsigned long long: argdata_get_int_ullong)(ad, value)
// clang-format on

#endif
