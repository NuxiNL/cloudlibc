// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#ifndef COMMON_OVERFLOW_H
#define COMMON_OVERFLOW_H

// Performs an addition, subtraction or multiplication operation,
// returning whether the computation caused an overflow. These
// intrinsics are available as of Clang 3.8 and GCC 5.
#define add_overflow(x, y, out) __builtin_add_overflow(x, y, out)
#define sub_overflow(x, y, out) __builtin_sub_overflow(x, y, out)
#define mul_overflow(x, y, out) __builtin_mul_overflow(x, y, out)

#endif
