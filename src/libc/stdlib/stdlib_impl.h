// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#ifndef STDLIB_STDLIB_IMPL_H
#define STDLIB_STDLIB_IMPL_H

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

#endif
