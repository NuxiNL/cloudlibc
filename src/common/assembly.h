// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#ifndef COMMON_ASSEMBLY_H
#define COMMON_ASSEMBLY_H

#if defined(__aarch64__)

#define ENTRY(name) \
  .text;            \
  .align 2;         \
  .global name;     \
  name:

#define END(name) .size name, .- name

#elif defined(__arm__)

#define ENTRY(name) \
  .text;            \
  .align 2;         \
  .global name;     \
  name:

#define END(name) .size name, .- name

#endif

#define ALTENTRY ENTRY

#endif
