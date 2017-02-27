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

#ifndef ARGDATA_H
#define ARGDATA_H

#include <limits.h>
#include <stdalign.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

struct timespec;

// The (opaque) type representing an argdata value.
// This library interface only ever exposes pointers to argdata_t, never
// argdata_t objects themselves.
#ifndef argdata_t
typedef struct argdata_t argdata_t;
#define argdata_t argdata_t
#endif

// The (mostly opaque) type representing an iterator into an argdata_t map.
// See argdata_map_iterate and argdata_map_next.
typedef struct {
  alignas(long) int error;
  char data[128];
} argdata_map_iterator_t;

// The (mostly opaque) type representing an iterator into an argdata_t sequence.
// See argdata_seq_iterate and argdata_seq_next.
typedef struct {
  alignas(long) int error;
  char data[128];
} argdata_seq_iterator_t;

// The global constant representing a false boolean value.
extern const argdata_t argdata_false;

// The global constant representing a true boolean value.
extern const argdata_t argdata_true;

// The global constant representing no value.
extern const argdata_t argdata_null;

#ifdef __cplusplus
extern "C" {
#endif

// Create a value from a buffer containing the serialized argdata.
// The data is not decoded, the get and iterate functions operate directly on
// the serialized data.
// The data is not copied, only the pointer and size are stored in the
// argdata_t object.
argdata_t *argdata_from_buffer(const void *, size_t);

// Create an argdata_t representing a binary value.
// The data is not copied, only the pointer and size are stored in the
// argdata_t object.
argdata_t *argdata_create_binary(const void *, size_t);

// Create an argdata_t representing a file descriptor.
argdata_t *argdata_create_fd(int);

// Create an argdata_t representing a floating point value.
argdata_t *argdata_create_float(double);

// Create an argdata_t representing an integer.
// Use argdata_create_int(any_integral_type) to call the right function, based
// on whether the type is unsigned or not.
argdata_t *argdata_create_int_s(intmax_t);
argdata_t *argdata_create_int_u(uintmax_t);

// Create an argdata_t representing a map (of key-value pairs of argdata_t).
// The given arrays are not copied. Only the pointers and the size is stored in
// the argdata_t object.
argdata_t *argdata_create_map(argdata_t const *const *keys,
                              argdata_t const *const *values,
                              size_t n_key_value_pairs);

// Create an argdata_t representing a sequence (of argdata_t).
// The given array is not copied. Only the pointer and the size is stored in
// the argdata_t object.
argdata_t *argdata_create_seq(argdata_t const *const *, size_t);

// Create an argdata_t representing a string.
// Returns NULL and sets errno to EILSEQ if the string is not valid UTF-8.
// The _c variant takes a null-terminated string, and calculates the size using
// strlen().
// When using the first version, argdata_get_str_c can only be used on the
// result if the buffer is followed by a null char. If you're only going to use
// argdata_serialize and argdata_get_str on the result, this is not an issue.
argdata_t *argdata_create_str(const char *, size_t);
argdata_t *argdata_create_str_c(const char *);

// Create an argdata_t representing a timestamp.
// A (serialized) copy of the timespec is made, so the original does not have
// to be kept around.
argdata_t *argdata_create_timestamp(const struct timespec *);

// Free an argdata_t.
// Any argdata_t created with an argdata_create_ function should be freed with
// this function.
void argdata_free(argdata_t *);

// Calculate the length of the serialization of the given argdata_t value.
// The size is stored in *size.
// If n_fds is not NULL, the number of file descriptor values (nested) in the
// argdata_t will be stored in *n_fds. This is not necessarily the same as the
// amount of unique file descriptors, as multiple values can refer to the same
// file descriptor.
void argdata_serialized_length(const argdata_t *, size_t *size, size_t *n_fds);

// Serialize the given argdata_t value into buf.
// The number of bytes needed and used in buf can be determined with
// argdata_serialized_length.
// If fds is not NULL, file descriptor values in this argdata_t will be remapped
// to the lowest possible values. The mapping is then stored in the array
// pointed
// at by fds: fds[0] will contain the file descriptor that was serialized as
// fd 0, etc. The number of unique file descriptors, which is the number of
// file descriptors stored in fds, is returned.
// Use *n_fds of argdata_serialize_length to get an upper bound on the number
// of unique file descriptors, to determine the size of the array.
// If fds is NULL, 0 is returned.
size_t argdata_serialize(const argdata_t *, void *buf, int *fds);

// Get the pointer and size to the binary data in the argdata_t.
// On success, returns 0.
// Returns EINVAL if the argdata_t value isn't of the binary type.
int argdata_get_binary(const argdata_t *, const void **, size_t *);

// Get the boolean value of the argdata_t.
// Returns 0 on success, or EINVAL if the argdata_t is not a boolean.
int argdata_get_bool(const argdata_t *, bool *);

// Get the file descriptor represented by the argdata_t.
// Returns 0 on success, or EINVAL if the argdata_t does not represent a file
// descriptor.
int argdata_get_fd(const argdata_t *, int *);

// Get the floating point value of the argdata_t.
// Returns 0 on success, or EINVAL if the argdata_t is not a float.
int argdata_get_float(const argdata_t *, double *);

// Get the integer value of the argdata_t.
// Returns 0 on success, EINVAL when the argdata_t isn't an integer,
// or ERANGE when the integer did not fit in the given range.
// Use argdata_get_int(const argdata_t *, any_integral_type *) to call the
// right function with the range of the used type.
int argdata_get_int_s(const argdata_t *, intmax_t *, intmax_t, intmax_t);
int argdata_get_int_u(const argdata_t *, uintmax_t *, uintmax_t);

// Get the string value of the argdata_t.
// Returns 0 on succes, EINVAL when the argdata_t isn't a string, or EILSEQ if
// the string is not valid UTF-8. The _c variant also returns EILSEQ if the
// string contains embedded null bytes.
// The _c variant shouldn't be used on argdata_t created by argdata_create_str,
// if the original buffer isn't followed by a null char.
int argdata_get_str(const argdata_t *, const char **, size_t *);
int argdata_get_str_c(const argdata_t *, const char **);

// Get the timestamp value of the argdata_t.
// Returns 0 on succes, ERANGE if the timestamp doesn't fit in a timespec, or
// EINVAL if the argdata_t isn't a timestamp.
int argdata_get_timestamp(const argdata_t *, struct timespec *);

// Initialize a argdata_map_iterator to iterate over the given map.
// Returns 0 on success, or EINVAL when the argdata_t isn't a map.
// On error, the iterator is still initialized, but to iterate over an empty
// sequence instead. The return value is also stored in it->error.
int argdata_map_iterate(const argdata_t *, argdata_map_iterator_t *it);

// Read the next element of a map.
// Use argdata_map_iterate to (re)initialize an iterator to (re)start iterating
// over a map.
// For every key-value pair, the key and value are stored in *key and *value,
// and true is returned. The returned *key and *value may be invalidated when
// the iterator is advanced or reset.
// When no more key-value pairs are left, false is returned.
// When corrupted data (data not encoding a subfield, or a key without a value)
// is encountered, false is returned and it->error is set to EINVAL.
bool argdata_map_next(argdata_map_iterator_t *it, const argdata_t **key,
                      const argdata_t **value);

// Initialize a argdata_seq_iterator to iterate over the given seq.
// Returns 0 on success, or EINVAL when the argdata_t isn't a seq.
// On error, the iterator is still initialized, but to iterate over an empty
// sequence instead. The return value is also stored in it->error.
int argdata_seq_iterate(const argdata_t *, argdata_seq_iterator_t *it);

// Read the next element of a seq.
// Use argdata_seq_iterate to (re)initialize an iterator to (re)start iterating
// over a seq.
// For every element, the element is stored in *element, and true is returned.
// The returned *element may be invalidated when the iterator is advanced or
// reset.
// When no more elements are left, false is returned.
// When corrupted data (data not encoding a subfield) is encountered, false is
// returned and it->error is set to EINVAL.
bool argdata_seq_next(argdata_seq_iterator_t *it, const argdata_t **element);

// Write a yaml representation of the argdata to the given file.
void argdata_print_yaml(const argdata_t *, FILE *);

#ifdef __cplusplus
}
#endif

// Definitions of argdata_create_int and argdata_get_int below.
// In C, macros and _Generic() are used.
// In C++, overloading is used.

#ifdef __cplusplus

#define ARGDATA_INT_OVERLOADS(type, stype, sign)                 \
  inline argdata_t *argdata_create_int(type value) {             \
    return argdata_create_int_##sign(value);                     \
  }                                                              \
  inline int argdata_get_int(const argdata_t *ad, type *value) { \
    return argdata_get_int_##stype(ad, value);                   \
  }

#else

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

#define ARGDATA_INT_OVERLOADS(type, stype, sign)

#endif

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
  }                                                              \
  ARGDATA_INT_OVERLOADS(type, stype, s)

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
  }                                                              \
  ARGDATA_INT_OVERLOADS(type, stype, u)

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

#undef ARGDATA_INT_OVERLOADS
#undef ARGDATA_INT_S
#undef ARGDATA_INT_U

#endif
