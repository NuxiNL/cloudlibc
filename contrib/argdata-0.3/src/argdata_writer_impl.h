// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#ifndef ARGDATA_WRITER_IMPL_H
#define ARGDATA_WRITER_IMPL_H

#include <stddef.h>
#include <stdint.h>

#include "argdata.h"

struct argdata_writer_t {
  const argdata_t *next;  // Next message to be transmitted.

  uint8_t *data;     // Part of the message that needs to be transmitted.
  size_t data_size;  // Remaining number of bytes to be transmitted.

  char *control;        // Control message that needs to be transmitted.
  size_t control_size;  // Size of the control message to be transmitted.

  size_t buffer_size;  // Size of the currently allocated write buffer.
};

#endif
