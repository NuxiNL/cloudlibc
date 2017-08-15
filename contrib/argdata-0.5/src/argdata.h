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
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#ifdef _MSC_VER
#define ARGDATA_MAX_ALIGN __declspec(align(8))
#else
#include <stdalign.h>
#define ARGDATA_MAX_ALIGN alignas(max_align_t)
#endif

struct timespec;

// The (opaque) type representing an argdata value.
// This library interface only ever exposes pointers to argdata_t, never
// argdata_t objects themselves.
#ifndef argdata_t
typedef struct argdata_t argdata_t;
#define argdata_t argdata_t
#endif

// The (mostly opaque) type representing an iterator into an argdata_t map.
// See argdata_map_iterate, argdata_map_get and argdata_map_next.
typedef struct {
  ARGDATA_MAX_ALIGN size_t index;
  char data[128];
} argdata_map_iterator_t;

// The (mostly opaque) type representing an iterator into an argdata_t sequence.
// See argdata_seq_iterate, argdata_seq_get and argdata_seq_next.
typedef struct {
  ARGDATA_MAX_ALIGN size_t index;
  char data[128];
} argdata_seq_iterator_t;

// Value for argdata_map_iterator_t and argdata_seq_iterator_t's index,
// indicating that the iterator is positioned past the last element in
// the map or sequence.
#define ARGDATA_ITERATOR_END SIZE_MAX

// Value for argdata_map_iterator_t and argdata_seq_iterator_t's index,
// indicating that the iterator is in an invalid state, due to it being
// unable to parse the map or sequence properly.
#define ARGDATA_ITERATOR_INVALID (SIZE_MAX - 1)

// A utility type for reading argdata values off any stream-based file
// descriptor (e.g., a regular file, pipe or socket).
#ifndef argdata_reader_t
typedef struct argdata_reader_t argdata_reader_t;
#define argdata_reader_t argdata_reader_t
#endif

// A utility type for writing argdata values to any stream-based file
// descriptor (e.g., a regular file, pipe or socket).
#ifndef argdata_writer_t
typedef struct argdata_writer_t argdata_writer_t;
#define argdata_writer_t argdata_writer_t
#endif

#ifdef __cplusplus
extern "C" {
#endif

// The global constant representing a false boolean value.
extern const argdata_t argdata_false;

// The global constant representing a true boolean value.
extern const argdata_t argdata_true;

// The global constant representing no value.
extern const argdata_t argdata_null;

// Create a value from a buffer containing the serialized argdata.
// The data is not decoded, the get and iterate functions operate directly on
// the serialized data.
// The data is not copied, only the pointer and size are stored in the
// argdata_t object.
// The convert_fd callback function is called when accessing file
// descriptors. This function can be used to transform a file descriptor
// number in the serialized data to its process-local value. In case
// convert_fd is NULL, access to file descriptors is disallowed entirely.
argdata_t *argdata_from_buffer(const void *buf, size_t len,
                               int (*convert_fd)(void *, size_t),
                               void *convert_fd_arg);

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
// Returns 0 on success, EINVAL if the argdata_t does not represent a
// file descriptor, or EBADF if the file descriptor associated with the
// object is not available within the current process.
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

// Initializes a map iterator to point to the first entry in a map.
// Stores 0 in it->index on success, ARGDATA_ITERATOR_END when the map
// is empty, or ARGDATA_ITERATOR_INVALID when the argdata_t isn't a map.
// In the latter two cases, calling argdata_map_get or argdata_map_next
// on the iterator will return failure, as with any past-the-end iterator.
void argdata_map_iterate(const argdata_t *, argdata_map_iterator_t *it);

// Extracts the key and value of the map entry currently referenced by
// the map iterator and stores them in *key and *value. Returns true if
// the iterator points to a valid map entry. Returns false if the
// iterator is past the final map entry.
bool argdata_map_get(const argdata_map_iterator_t *it, const argdata_t **key,
                     const argdata_t **value);

// Progresses a map iterator to the next element in the map. Use
// argdata_map_iterate to (re)initialize an iterator to (re)start iterating
// over a map. When iterating past the last element, it->index is set to
// ARGDATA_ITERATOR_END. When corrupted data (data not encoding a
// subfield, or a key without a value) is encountered, it->index is set
// to ARGDATA_ITERATOR_INVALID.
void argdata_map_next(argdata_map_iterator_t *it);

// Initializes a sequence iterator to point to the first element of a sequence.
// Stores 0 in it->index on success, ARGDATA_ITERATOR_END when the
// sequence is empty, or ARGDATA_ITERATOR_INVALID when the argdata_t
// isn't a sequence. In the latter two cases, calling argdata_seq_get or
// argdata_seq_next on the iterator will return failure, as with any
// past-the-end iterator.
void argdata_seq_iterate(const argdata_t *, argdata_seq_iterator_t *it);

// Extracts the value of the sequence entry currently referenced by the
// map iterator and stores it in *element. Returns true if the iterator
// points to a valid sequence entry. Returns false if the iterator is
// past the final sequence entry.
bool argdata_seq_get(const argdata_seq_iterator_t *it,
                     const argdata_t **element);

// Progresses a sequence iterator to the next element in the sequence.
// Use argdata_seq_iterate to (re)initialize an iterator to (re)start
// iterating over a sequence. When iterating past the last element,
// it->index is set to ARGDATA_ITERATOR_END. When corrupted data (data
// not encoding a subfield, or a key without a value) is encountered,
// it->index is set to ARGDATA_ITERATOR_INVALID.
void argdata_seq_next(argdata_seq_iterator_t *it);

// Write a yaml representation of the argdata to the given file.
void argdata_print_yaml(const argdata_t *, FILE *);

// Creates a reader object, capable of reading argdata values from any
// stream-based file descriptor, storing a single message at a time. The
// maximum message size and number of file descriptors need to be
// specified to prevent potential resource exhaustion.
argdata_reader_t *argdata_reader_create(size_t max_data_len,
                                        size_t max_fds_len);

// Destroys a reader object, freeing the last read message and any
// unreleased file descriptors (if any).
void argdata_reader_free(argdata_reader_t *);

// Returns the last argdata value read from a stream. This function can
// only be called if the last call to argdata_reader_pull() returned
// zero. This function returns NULL if argdata_reader_pull() encountered
// end-of-file.
const argdata_t *argdata_reader_get(const argdata_reader_t *);

// Releases ownership of a file descriptor contained in the last argdata
// value, so that it can continue to be used after reading the next
// value or after destroying the reader. This function may only be
// called on file descriptors that are owned by the reader. Future calls
// to argdata_get_fd() on elements corresponding to the file descriptor
// will fail with EBADF.
void argdata_reader_release_fd(argdata_reader_t *, int);

// Reads the next argdata value (possibly partially) from a stream-based
// file descriptor. This function returns zero upon completing a message
// or when reaching end-of-file while in the initial state. Calling this
// function invalidates the previously read value and closes any
// associated file descriptors that have not been released through
// argdata_reader_release_fd().
//
// This function may return any error number returned by read() and
// recvmsg(). Errors like EAGAIN and EWOULDBLOCK indicate that the
// next message has only been read partially. In addition, it may return
// EMSGSIZE when the message is too large to be processed by the reader.
// EBADMSG may be returned when the message is malformed or truncated.
int argdata_reader_pull(argdata_reader_t *, int);

// Creates a writer object, capable of writing argdata values to any
// stream-based file descriptor, storing a single (partially
// transmitted) message at a time.
argdata_writer_t *argdata_writer_create(void);

// Destroys a writer object.
void argdata_writer_free(argdata_writer_t *);

// Writes the current argdata value (possibly partially) to a
// stream-based file descriptor. This function returns zero upon
// completing the transmission of the current message or if no message
// has been set.
//
// This function may return any error number returned by write() and
// sendmsg(). Errors like EAGAIN and EWOULDBLOCK indicate that the
// current message has only been written partially. ENOTSOCK is returned
// when attempting to write a message containing file descriptors
// through a non-socket file descriptor.
int argdata_writer_push(argdata_writer_t *, int);

// Starts the transmission of a message. This function can only be
// called once when the writer object is in the initial state or when
// the previous call to argdata_writer_push() returned zero.
void argdata_writer_set(argdata_writer_t *, const argdata_t *);

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
