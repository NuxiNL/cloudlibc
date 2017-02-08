// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <execinfo.h>
#include <stddef.h>

// Simple stack frame structure.
struct frame {
  struct frame *next;    // Stack frame of the caller.
  void *return_address;  // Return address into the caller.
};

// Tolerances of when to consider successive frames invalid.
#define NEGATIVE_TOLERANCE 0
#define POSITIVE_TOLERANCE (1 << 16)

size_t backtrace(void **buffer, size_t size) {
  struct frame *f = __builtin_frame_address(0);
  for (size_t i = 0; i < size; ++i) {
    buffer[i] = f->return_address;
    if ((char *)f->next <= (char *)f - NEGATIVE_TOLERANCE ||
        (char *)f->next >= (char *)f + POSITIVE_TOLERANCE)
      return i + 1;
    f = f->next;
  }
  return size;
}
