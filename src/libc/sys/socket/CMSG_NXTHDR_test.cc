// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/socket.h>

#include <stddef.h>

#include "gtest/gtest.h"

TEST(CMSG_NXTHDR, first) {
  // No ancillary data.
  {
    struct msghdr mhdr = {};
    ASSERT_EQ(NULL, CMSG_NXTHDR(&mhdr, NULL));
  }

  // Zero-sized ancillary data.
  {
    struct cmsghdr cmsg;
    struct msghdr mhdr = {.msg_control = &cmsg};
    ASSERT_EQ(NULL, CMSG_NXTHDR(&mhdr, NULL));
  }

  // Ancillary data is too small to fit any data.
  {
    struct cmsghdr cmsg;
    struct msghdr mhdr = {
        .msg_control = &cmsg,
        .msg_controllen = CMSG_LEN(0) - 1,
    };
    ASSERT_EQ(NULL, CMSG_NXTHDR(&mhdr, NULL));
  }

  // Ancillary data is just large enough.
  {
    struct cmsghdr cmsg;
    struct msghdr mhdr = {
        .msg_control = &cmsg,
        .msg_controllen = CMSG_LEN(0),
    };
    ASSERT_EQ(&cmsg, CMSG_NXTHDR(&mhdr, NULL));
  }
}

TEST(CMSG_NXTHDR, second) {
  for (size_t i = 0; i < 100; ++i) {
    size_t firstlen = CMSG_SPACE(i);
    alignas(struct cmsghdr) char cmsg[firstlen + CMSG_LEN(0)];

    // Ancillary data is too small to fit a second element.
    {
      struct msghdr mhdr = {
          .msg_control = cmsg,
          .msg_controllen = firstlen + CMSG_LEN(0) - 1,
      };
      struct cmsghdr *first = CMSG_NXTHDR(&mhdr, NULL);
      first->cmsg_len = CMSG_LEN(i);
      ASSERT_EQ(static_cast<void *>(cmsg), static_cast<void *>(first));
      ASSERT_EQ(NULL, CMSG_NXTHDR(&mhdr, first));
    }

    // Ancillary data is just large enough to fit a second element.
    {
      struct msghdr mhdr = {
          .msg_control = cmsg,
          .msg_controllen = firstlen + CMSG_LEN(0),
      };
      struct cmsghdr *first = CMSG_NXTHDR(&mhdr, NULL);
      ASSERT_EQ(static_cast<void *>(cmsg), static_cast<void *>(first));
      first->cmsg_len = CMSG_LEN(i);
      ASSERT_EQ(static_cast<void *>(cmsg + firstlen),
                static_cast<void *>(CMSG_NXTHDR(&mhdr, first)));
    }
  }
}

TEST(CMSG_NXTHDR, hundred) {
  // Create a message containing 100 control message headers.
  alignas(struct cmsghdr) char cmsg[100][CMSG_SPACE(10)];
  struct msghdr mhdr = {
      .msg_control = cmsg,
      .msg_controllen = sizeof(cmsg),
  };

  // Iterate through all of them.
  struct cmsghdr *cur = NULL;
  for (size_t i = 0; i < 100; ++i) {
    cur = CMSG_NXTHDR(&mhdr, cur);
    ASSERT_EQ(static_cast<void *>(cmsg[i]), static_cast<void *>(cur));
    cur->cmsg_len = CMSG_LEN(10);
  }
  ASSERT_EQ(NULL, CMSG_NXTHDR(&mhdr, cur));
}
