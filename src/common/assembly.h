// Copyright (c) 2015 Nuxi, https://nuxi.nl/
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

#elif defined(__i386__)

#define ENTRY(name)      \
  .text;                 \
  .p2align 2, 0x90;      \
  .global name;          \
  .type name, @function; \
  name:

#define END(name) .size name, .- name

#elif defined(__x86_64__)

#define ENTRY(name)      \
  .text;                 \
  .p2align 4, 0x90;      \
  .global name;          \
  .type name, @function; \
  name:

#define END(name) .size name, .- name

#else

#error "Assembly macros not implemented for this architecture"

#endif

#define ALTENTRY ENTRY

#endif
