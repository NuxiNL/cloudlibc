// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <arpa/inet.h>

#include <netdb.h>
#include <stddef.h>

#include "gtest/gtest.h"

TEST(getservbyport, bad) {
  ASSERT_EQ(NULL, getservbyport(htons(80), "badproto"));
  ASSERT_EQ(NULL, getservbyport(htons(0), "tcp"));
  ASSERT_EQ(NULL, getservbyport(htons(80) + 65536, "tcp"));
}

TEST(getservbyport, http) {
  struct servent *se = getservbyport(htons(80), "TCP");
  ASSERT_STREQ("http", se->s_name);
  ASSERT_STREQ("www", se->s_aliases[0]);
  ASSERT_STREQ("www-http", se->s_aliases[1]);
  ASSERT_EQ(NULL, se->s_aliases[2]);
  ASSERT_EQ(ntohs(80), se->s_port);
  ASSERT_STREQ("tcp", se->s_proto);
}

TEST(getservbyport, https) {
  struct servent *se = getservbyport(htons(443), "tcp");
  ASSERT_STREQ("https", se->s_name);
  ASSERT_EQ(NULL, se->s_aliases[0]);
  ASSERT_EQ(ntohs(443), se->s_port);
  ASSERT_STREQ("tcp", se->s_proto);
}
