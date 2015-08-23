// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/socket.h>

#include <netinet/in.h>

#include <netdb.h>
#include <stddef.h>
#include <testing.h>

TEST(getnameinfo, bad) {
  // Bad flags value.
  struct sockaddr_in sin = {.sin_family = AF_INET};
  ASSERT_EQ(EAI_BADFLAGS, getnameinfo((struct sockaddr *)&sin, sizeof(sin),
                                      NULL, 0, NULL, 0, 0xdeadc0de));

  // Unsupported family.
  struct sockaddr sa = {.sa_family = AF_UNSPEC};
  ASSERT_EQ(EAI_FAMILY, getnameinfo(&sa, sizeof(sa), (char *)42, 0, (char *)42,
                                    0, NI_NUMERICHOST | NI_NUMERICSCOPE));

  // Nothing to do: both node and service are NULL.
  ASSERT_EQ(EAI_NONAME, getnameinfo(NULL, 0, NULL, 0, NULL, 0, 0));
}

#define TEST_SOCKADDR(sa, flags, node, service)                                \
  do {                                                                         \
    /* Perform conversion. */                                                  \
    char nodebuf[sizeof(node)];                                                \
    char servicebuf[sizeof(service)];                                          \
    ASSERT_EQ(0, getnameinfo((struct sockaddr *) & (sa), sizeof(sa), nodebuf,  \
                             sizeof(nodebuf), servicebuf, sizeof(servicebuf),  \
                             NI_NUMERICHOST | NI_NUMERICSCOPE | (flags)));     \
    ASSERT_STREQ(node, nodebuf);                                               \
    ASSERT_STREQ(service, servicebuf);                                         \
                                                                               \
    /* Test what happens if the buffers are too small. */                      \
    ASSERT_EQ(EAI_FAMILY,                                                      \
              getnameinfo((struct sockaddr *) & (sa), sizeof(sa) - 1, nodebuf, \
                          sizeof(nodebuf), servicebuf, sizeof(servicebuf),     \
                          NI_NUMERICHOST | NI_NUMERICSCOPE | (flags)));        \
    ASSERT_EQ(EAI_OVERFLOW,                                                    \
              getnameinfo((struct sockaddr *) & (sa), sizeof(sa), nodebuf,     \
                          sizeof(nodebuf) - 1, servicebuf, sizeof(servicebuf), \
                          NI_NUMERICHOST | NI_NUMERICSCOPE | (flags)));        \
    ASSERT_EQ(EAI_OVERFLOW,                                                    \
              getnameinfo((struct sockaddr *) & (sa), sizeof(sa), nodebuf,     \
                          sizeof(nodebuf), servicebuf, sizeof(servicebuf) - 1, \
                          NI_NUMERICHOST | NI_NUMERICSCOPE | (flags)));        \
  } while (0)

TEST(getnameinfo, inet) {
#define TEST_INET(addr, port, flags, node, service)                         \
  do {                                                                      \
    /* Perform conversion. */                                               \
    struct sockaddr_in sin = {                                              \
        .sin_family = AF_INET,                                              \
        .sin_addr.s_addr = htonl(addr),                                     \
        .sin_port = htons(port),                                            \
    };                                                                      \
    TEST_SOCKADDR(sin, flags, node, service);                               \
                                                                            \
    /* Also test IPv4-mapped IPv6 address representation. */                \
    struct sockaddr_in6 sin6 = {                                            \
        .sin6_family = AF_INET6,                                            \
        .sin6_addr.s6_addr =                                                \
            {                                                               \
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, \
                0xff, 0xff, (addr >> 24) & 0xff, (addr >> 16) & 0xff,       \
                (addr >> 8) & 0xff, addr & 0xff,                            \
            },                                                              \
        .sin6_port = htons(port)};                                          \
    TEST_SOCKADDR(sin6, flags, "::ffff:" node, service);                    \
  } while (0)
  TEST_INET(0x00000000, 0, NI_NUMERICSERV, "0.0.0.0", "0");
  TEST_INET(0x00000000, 0, 0, "0.0.0.0", "0");
  TEST_INET(0x00000000, 0, NI_DGRAM, "0.0.0.0", "0");

  TEST_INET(0x0808b287, 22, NI_NUMERICSERV, "8.8.178.135", "22");
  TEST_INET(0x0808b287, 22, 0, "8.8.178.135", "ssh");
  TEST_INET(0x0808b287, 22, NI_DGRAM, "8.8.178.135", "ssh");

  TEST_INET(0x00000000, 80, NI_NUMERICSERV, "0.0.0.0", "80");
  TEST_INET(0x00000000, 80, 0, "0.0.0.0", "http");
  TEST_INET(0x00000000, 80, NI_DGRAM, "0.0.0.0", "http");

  TEST_INET(0x7f000001, 513, NI_NUMERICSERV, "127.0.0.1", "513");
  TEST_INET(0x7f000001, 513, 0, "127.0.0.1", "login");
  TEST_INET(0x7f000001, 513, NI_DGRAM, "127.0.0.1", "who");
#undef TEST_INET
}

// TODO(ed): Test IPv6.
