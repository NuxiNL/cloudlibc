// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#ifndef COMMON_FLOAT10_H
#define COMMON_FLOAT10_H

#include <stddef.h>

// Handling of base-10 floating point numbers. These functions are
// wrappers around the double-conversion library.

__BEGIN_DECLS
float __f10enc_get_float(const char *, size_t, int);
double __f10enc_get_double(const char *, size_t, int);
long double __f10enc_get_long_double(const char *, size_t, int);
void __f10dec(long double, unsigned int, unsigned char *, size_t *, int *, int);
__END_DECLS

#endif
