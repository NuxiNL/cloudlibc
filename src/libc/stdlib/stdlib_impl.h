// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#ifndef STDLIB_STDLIB_IMPL_H
#define STDLIB_STDLIB_IMPL_H

#include <stdint.h>

struct atexit {
  // Handler registered by atexit().
  void (*atexit)(void);

  // Handler registered by __cxa_atexit().
  void (*cxa_atexit)(void *);
  void *cxa_atexit_arg;

  // Previous list element.
  struct atexit *previous;
};

// Pointer to last atexit() handler.
extern _Atomic(struct atexit *) __atexit_last;

struct at_quick_exit {
  // Handler registered by at_quick_exit().
  void (*func)(void);

  // Previous list element.
  struct at_quick_exit *previous;
};

// Pointer to last at_quick_exit() handler.
extern _Atomic(struct at_quick_exit *) __at_quick_exit_last;

// Linear congruential generator used by *rand48().
static inline uint_least48_t lcg48(uint16_t *xsubi) {
  uint_least48_t v = (uint_least48_t)xsubi[0] | (uint_least48_t)xsubi[1] << 16 |
                     (uint_least48_t)xsubi[2] << 32;
  v = (v * 0x5deece66d + 0xb) & 0xffffffffffff;
  xsubi[0] = v;
  xsubi[1] = v >> 16;
  xsubi[2] = v >> 32;
  return v;
}

#endif
