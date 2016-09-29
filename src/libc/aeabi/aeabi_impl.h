// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#ifndef AEABI_AEABI_IMPL_H
#define AEABI_AEABI_IMPL_H

#include <stddef.h>

#ifdef __arm__

void __aeabi_memclr(void *, size_t);
void __aeabi_memclr4(void *, size_t);
void __aeabi_memclr8(void *, size_t);
void __aeabi_unwind_cpp_pr0(void);
void __aeabi_unwind_cpp_pr1(void);

#endif

#endif
