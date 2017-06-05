// Copyright (c) 2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/socket.h>

#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include "argdata_writer_impl.h"

int argdata_writer_push(argdata_writer_t *aw, int fd) {
  if (aw->next != NULL) {
    // Obtain upper bound on message to be serialized.
    size_t data_len, fds_len;
    argdata_serialized_length(aw->next, &data_len, &fds_len);

    // Grow the internally allocated buffer to fit the serialized message.
    size_t max_control_size = CMSG_SPACE(fds_len * sizeof(int));
    size_t data_size = 8 + data_len;
    size_t minimum_size = max_control_size + data_size;
    if (aw->buffer_size < minimum_size) {
      size_t new_buffer_size = 32;
      while (new_buffer_size < minimum_size)
        new_buffer_size *= 2;
      char *new_buffer = malloc(new_buffer_size);
      if (new_buffer == NULL)
        return ENOMEM;
      free(aw->control);
      aw->control = new_buffer;
      aw->buffer_size = new_buffer_size;
    }

    // Serialize the message, placing the file descriptors to be
    // transmitted in a control message.
    aw->data = (uint8_t *)(aw->control + max_control_size);
    struct msghdr msg = {
        .msg_control = aw->control,
        .msg_controllen = CMSG_SPACE(fds_len * sizeof(int)),
    };
    struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);
    fds_len = argdata_serialize(aw->next, aw->data + 8, (int *)CMSG_DATA(cmsg));

    // Construct the message header, containing an eight-byte data length.
    aw->data_size = data_size;
    static_assert(SIZE_MAX <= UINT64_MAX, "Message size cannot be encoded");
    aw->data[0] = data_len >> 56;
    aw->data[1] = data_len >> 48;
    aw->data[2] = data_len >> 40;
    aw->data[3] = data_len >> 32;
    aw->data[4] = data_len >> 24;
    aw->data[5] = data_len >> 16;
    aw->data[6] = data_len >> 8;
    aw->data[7] = data_len;

    // Only provide a control message when sending file descriptors.
    if (fds_len > 0) {
      aw->control_size = CMSG_SPACE(fds_len * sizeof(int));
      cmsg->cmsg_len = CMSG_LEN(fds_len * sizeof(int));
      cmsg->cmsg_level = SOL_SOCKET;
      cmsg->cmsg_type = SCM_RIGHTS;
    } else {
      aw->control_size = 0;
    }

    aw->next = NULL;
  }

  // Write message.
  while (aw->data_size > 0) {
    ssize_t ret;
    if (aw->control_size > 0) {
      // Message has a control message. Use sendmsg().
      struct iovec iov = {
          .iov_base = aw->data, .iov_len = aw->data_size,
      };
      struct msghdr msg = {
          .msg_iov = &iov,
          .msg_iovlen = 1,
          .msg_control = aw->control,
          .msg_controllen = aw->control_size,
      };
      ret = sendmsg(fd, &msg, 0);
    } else {
      // Message has no control message. Use write(), so that we can
      // also write to regular files and pipes.
      ret = write(fd, aw->data, aw->data_size);
    }
    if (ret == -1)
      return errno;

    aw->data += ret;
    aw->data_size -= ret;
    aw->control_size = 0;
  }
  return 0;
}
