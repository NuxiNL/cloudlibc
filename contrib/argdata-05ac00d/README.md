# Argdata

Argdata is a binary serialisation format inspired by YAML, [MessagePack][1] and
[nvlists][2].

It is originally designed and currently used in [CloudABI][3] but does not
depend on anything specific to CloudABI.

The encoding is optimized for reading:

 - Strings (UTF-8 encoded) and binary data appear verbatim within the encoded
   data. Strings are followed by a null byte in the encoded data. This way,
   pointers into the encoded argdata can be used directly, without having to
   copy or overwrite data.

 - Instead of encoding the number of elements in maps and sequences (the only
   two structures containing subfields), the length of the subfields (in bytes)
   is stored before each subfield. This makes it possible to iterate over a
   map or sequence, without having to recursively iterate depth-first through
   all the elements. Skipping over a large map containing many other maps is as
   quick as skipping over a single integer. This means the encoded data can be
   navigated through quite efficiently, without constructing a data structure
   next to the encoded data.

So, after the encoded argdata is stored in a buffer, no dynamic memory is needed
at all to use any of the values contained within.

[1]: https://msgpack.org/
[2]: https://www.freebsd.org/cgi/man.cgi?query=libnv
[3]: https://nuxi.nl/cloudabi

## Types

Argdata values have one of the following types:

 - null - Nothing.
 - binary - A sequence of bytes.
 - string - An UTF-8 string.
 - bool - Either `true` or `false`.
 - int - A signed integer of any size.
 - float - A (64-bit) floating point value.
 - map - A sequence of keys with values.
   Keys and values can both be any argdata value.
 - seq - A sequence of argdata values.
 - timestamp - A point in time. (In UTC, no timezone information.)
 - fd - A file descriptor number, used on POSIX and CloudABI systems.

## Library

This repository contains a C library for handling argdata. See the
(well-commented) [argdata.h](src/argdata.h) for the details. A C++ interface is
available for the same library in [argdata.hpp](src/argdata.hpp).

## Binary encoding

The first byte of an encoded argdata value encodes the type of the value, using
the tag value in the table below. Unlike many other serialization formats,
encoded argdata values do not encode their own length. It is assumed that
the length of the full buffer is already known. The length of the value is
simply the length of the full buffer, minus the one tag byte encoding the type.

Null values do not have any tag, they don't even take any space at all: They
are encoded as ` `, nothing, zero bytes.

| Type      | Tag  | Value bytes
|-----------|------|----
| binary    | `01` | the binary data
| bool      | `02` | false: none, true: `01`
| fd        | `03` | 32-bit big-endian integer (e.g. `00 00 00 02` for fd 2)
| float     | `04` | 64-bit big-endian IEEE754 float
| int       | `05` | N-bit signed big-endian integer (see below)
| map       | `06` | repeated subfield (see below): key, value, key, value, ...
| seq       | `07` | repeated subfield (see below): value, value, value, ...
| string    | `08` | null-terminated UTF-8
| timestamp | `09` | N-bit signed big-endian integer (see below), encoding nanoseconds since 1970 (UTC)

For example, the string `"123"` is encoded as `08 31 32 33 00`: The tag byte
`08` for the type, followed by the UTF-8 encoding of the string, followed by a
terminating null byte.

### N-bit signed integers

Integers are signed, and encoded big-endian in as few bytes as needed. The C
library doesn't support decoding and encoding integers that don't fit in
`intmax_t` or `uintmax_t` (usually 64-bit), but the binary format has no
restrictions on the size of integers.

Some examples:

| Value    | Bytes
|----------|-----
| 0        | none
| 1        | `01`
| 127      | `7F`
| -128     | `80`
| -1       | `FF`
| 255      | `00 FF`
| 1000     | `03 E8`
| -1000    | `FC 18`
| 2^32 - 1 | `00 FF FF FF FF`

Since integers are always encoded using the least bytes possible, no integer
should be encoded as, for example, `00` or `FF FF`. (Since the values 0 and -1
can be encoded as an empty sequence and `FF`, respectively.)

### Subfields

Subfields, inside the map and seq types, are encoded by their length, followed
by the encoded value itself. The length is encoded as a variable length
unsigned big-endian integer, where the high bit of each byte is not part of the
integer, but indicates whether the byte is the last byte of the integer.

For example, the length of a subfield of 6 bytes is encoded as `86`: The high
bit is on, indicating that this is the last byte of the variable length
integer, and the other 7 bits encode the value 6. As another example, a length
of 128 is encoded as `01 80`: Only the high bit of the second byte is set,
indicating that two bytes are used, and the other 14 bits (7 bits of the first,
and 7 bits of the second byte) encode the value 128.

Maps and sequences don't encode their own length, or number of elements stored.
The subfields simply end when there are no more bytes left.

Maps always have an even number of subfields, as every pair represents a key
with its value.

For example, a sequence of values 0, true, and "A" is encoded as:
`07 81 05 82 02 01 83 08 41 00`:

| Bytes   | Meaning
|---------|--------
| `07`    | seq type tag
| `81`    | length of first subfield: 1
| `05`    | first subfield: the integer `0`
| `82`    | length of second subfield: 2
| `02 01` | second subfield: the boolean value `true`
| `83`    | length of the third subfield: 3
| `08 41 00` | third subfield: the string "A" (with null terminator)

### File Descriptors

File descriptor numbers in argdata refer to open file descriptors in the same
process or passed along with the message that contained the argdata. This is not
useful on every platform, but can for example be used on CloudABI and POSIX
systems.

File descriptors are always stored as an 32-bit integer in exactly four bytes,
as opposed to integers which use only as many bytes as needed. The reason for
this, is to allow substitution of file descriptors in encoded argdata. If the
file descriptors were variable length, changing the value could involve
resizing and thus re-encoding (part of) the argdata. Being able to substitute
the these values is useful since in many cases file descriptor numbers cannot
be chosen freely.
