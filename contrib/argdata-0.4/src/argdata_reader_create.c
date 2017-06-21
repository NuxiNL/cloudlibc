// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stddef.h>
#include <stdlib.h>

#include "argdata_reader_impl.h"

argdata_reader_t *argdata_reader_create(size_t max_data_len,
                                        size_t max_fds_len) {
  argdata_reader_t *ar = malloc(
      offsetof(argdata_reader_t,
               control[CMSG_SPACE(max_fds_len * sizeof(int)) + max_data_len]));
  if (ar == NULL)
    return NULL;

  ar->max_data_len = max_data_len;
  ar->max_fds_len = max_fds_len;
  ar->finished = false;
  ar->header_len = 0;
  ar->data_len = 0;
  ar->fds_len = 0;
  return ar;
}
