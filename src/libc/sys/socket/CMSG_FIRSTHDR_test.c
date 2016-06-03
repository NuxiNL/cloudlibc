// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/socket.h>

#include <stddef.h>
#include <testing.h>

TEST(CMSG_FIRSTHDR, examples) {
  // No ancillary data.
  {
    struct msghdr mhdr = {};
    ASSERT_EQ(NULL, CMSG_FIRSTHDR(&mhdr));
  }

  // Zero-sized ancillary data.
  {
    struct cmsghdr cmsg;
    struct msghdr mhdr = {.msg_control = &cmsg};
    ASSERT_EQ(NULL, CMSG_FIRSTHDR(&mhdr));
  }

  // Ancillary data too is small to fit any data.
  {
    struct cmsghdr cmsg;
    struct msghdr mhdr = {
        .msg_control = &cmsg, .msg_controllen = CMSG_LEN(0) - 1,
    };
    ASSERT_EQ(NULL, CMSG_FIRSTHDR(&mhdr));
  }

  // Ancillary data is just large enough.
  {
    struct cmsghdr cmsg;
    struct msghdr mhdr = {
        .msg_control = &cmsg, .msg_controllen = CMSG_LEN(0),
    };
    ASSERT_EQ(&cmsg, CMSG_FIRSTHDR(&mhdr));
  }
}
