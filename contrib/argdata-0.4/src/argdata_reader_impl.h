// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#ifndef ARGDATA_READER_IMPL_H
#define ARGDATA_READER_IMPL_H

#include <sys/socket.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "argdata.h"
#include "argdata_impl.h"

struct argdata_reader_t {
  size_t max_data_len;  // Maximum permitted message size.
  size_t max_fds_len;   // Maximum number of files in message.

  bool finished;   // Message present or at end-of-file.
  argdata_t root;  // The last received message.

  uint8_t header[8];  // Bytes of the header received.
  size_t header_len;  // Number of bytes of the header read.
  size_t data_len;    // Number of bytes of the message read.
  int *fds;           // Process-local file descriptors in message.
  size_t fds_len;     // Number of file descriptors received.

  _Alignas(struct cmsghdr) char control[];  // Control message and message data.
};

#endif
