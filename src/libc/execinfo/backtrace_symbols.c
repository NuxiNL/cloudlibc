// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "execinfo_impl.h"

char **backtrace_symbols(void *const *buffer, size_t size) {
  // Corner case: make this function work with an empty input list.
  if (size == 0)
    return malloc(1);

  size_t target_size = 1;
  size_t target_offset = size * sizeof(char *);
  void *target = NULL;
  for (size_t i = 0; i < size; ++i) {
    // Determine length of textual representation.
    struct frame f;
    frame_get(&f, buffer[i]);
    size_t len = FRAME_PRINT(&f, snprintf, "", NULL, 0) + 1;

    // Grow output buffer if needed.
    if (target_offset + len > target_size) {
      do {
        target_size *= 2;
      } while (target_offset + len > target_size);
      char *new_target = realloc(target, target_size);
      if (new_target == NULL) {
        free(target);
        return NULL;
      }
      target = new_target;
    }

    // Concatenate textual representation to output.
    target_offset +=
        FRAME_PRINT(&f, snprintf, "", (char *)target + target_offset, len) + 1;
  }

  // Fill the start of the buffer with an array of pointers to strings.
  char **strings = target;
  strings[0] = (char *)target + size * sizeof(char *);
  for (size_t i = 1; i < size; ++i)
    strings[i] = strings[i - 1] + strlen(strings[i - 1]) + 1;
  return strings;
}
