// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <arpa/inet.h>

#include <netdb.h>
#include <stddef.h>

#include "gtest/gtest.h"

TEST(getservbyname, bad) {
  ASSERT_EQ(NULL, getservbyname("http", "badproto"));
  ASSERT_EQ(NULL, getservbyname("badservice", "tcp"));
}

TEST(getservbyname, http) {
  struct servent *se = getservbyname("Http", "Tcp");
  ASSERT_STREQ("http", se->s_name);
  ASSERT_STREQ("www", se->s_aliases[0]);
  ASSERT_STREQ("www-http", se->s_aliases[1]);
  ASSERT_EQ(NULL, se->s_aliases[2]);
  ASSERT_EQ(ntohs(80), se->s_port);
  ASSERT_STREQ("tcp", se->s_proto);
}

TEST(getservbyname, memoization) {
  struct servent *se = getservbyname("http", "tcp");
  ASSERT_NE(NULL, se);

  // Second invocation should return the same object.
  ASSERT_EQ(se, getservbyname("http", "tcp"));
  ASSERT_EQ(se, getservbyname("www", "tcp"));
  ASSERT_EQ(se, getservbyname("www-http", "tcp"));

  // Different services should use different objects.
  ASSERT_NE(se, getservbyname("ssh", "tcp"));
  ASSERT_NE(se, getservbyname("http", "udp"));
  ASSERT_NE(se, getservbyname("domain", "udp"));
}
