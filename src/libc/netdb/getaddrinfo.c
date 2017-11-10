// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/overflow.h>

#include <sys/socket.h>

#include <arpa/inet.h>

#include <netinet/in.h>

#include <netdb.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <strings.h>

#include "netdb_impl.h"

// Internal version of 'struct addrinfo' that contains storage for the
// socket address pointed to by struct addrinfo::ai_addr.
struct entry {
  struct addrinfo ai;
  union {
    struct sockaddr sa;
    struct sockaddr_in sin;
    struct sockaddr_in6 sin6;
  } addr;
};

// A service name might be registered under several distinct port
// numbers and network protocols. This structure holds the information
// for every alias.
struct alias {
  int socktype;
  int protocol;
  in_port_t port;
};

// Maximum number of aliases. The IANA port number database contains at
// most six entries for the same name (e.g., "http-alt"). To be on the
// safe side, pick a larger limit.
#define NALIASES_MAX 16

// Parses an IPv6 address using inet_pton(), but also extracts a
// trailing decimal scope identifier having the format "%n".
static int inet6_pton(const char *nodename, struct in6_addr *addr,
                      uint32_t *scope) {
  // The general case: no scope present.
  if (inet_pton(AF_INET6, nodename, addr) != 0) {
    *scope = 0;
    return 1;
  }

  // A trailing scope number may be present.
  const char *s = strchr(nodename, '%');
  if (s++ == NULL)
    return 0;

  // Call inet_pton() on the address.
  size_t addrlen = s - nodename;
  if (addrlen > INET6_ADDRSTRLEN)
    return 0;
  char nodeaddr[INET6_ADDRSTRLEN];
  memcpy(nodeaddr, nodename, addrlen - 1);
  nodeaddr[addrlen - 1] = '\0';
  if (inet_pton(AF_INET6, nodeaddr, addr) == 0)
    return 0;

  // Parse the scope number.
  *scope = 0;
  do {
    if (*s < '0' || *s > '9' || mul_overflow(*scope, 10, scope) ||
        add_overflow(*scope, *s - '0', scope))
      return 0;
  } while (*++s != '\0');
  return 1;
}

int getaddrinfo(const char *restrict nodename, const char *restrict servname,
                const struct addrinfo *restrict hints,
                struct addrinfo **restrict res) {
  // Fall back to default hints if none are provided.
  static const struct addrinfo default_hints = {.ai_family = AF_UNSPEC};
  if (hints == NULL)
    hints = &default_hints;

  // Validate flags.
  if ((hints->ai_flags & ~(AI_PASSIVE | AI_NUMERICSERV)) != 0)
    return EAI_BADFLAGS;

  // Either a node name or a service name has to be provided.
  if (nodename == NULL && servname == NULL)
    return EAI_NONAME;

  // Determine which address families to return.
  bool have_inet, have_inet6;
  switch (hints->ai_family) {
    case AF_UNSPEC:
      have_inet = true;
      have_inet6 = true;
      break;
    case AF_INET:
      have_inet = true;
      have_inet6 = false;
      break;
    case AF_INET6:
      have_inet = false;
      have_inet6 = true;
      break;
    default:
      return EAI_FAMILY;
  }

  // Attempt to parse as an IPv4 address.
  struct in_addr addr_inet;
  if (have_inet) {
    if (nodename == NULL) {
      addr_inet.s_addr = htonl(
          (hints->ai_flags & AI_PASSIVE) != 0 ? INADDR_ANY : INADDR_LOOPBACK);
    } else if (inet_aton(nodename, &addr_inet) == 0) {
      have_inet = false;
    }
  }

  // Attempt to parse as an IPv6 address.
  struct in6_addr addr_inet6;
  uint32_t scope_inet6 = 0;
  if (have_inet6) {
    if (nodename == NULL) {
      addr_inet6 = (hints->ai_flags & AI_PASSIVE) != 0
                       ? (struct in6_addr)IN6ADDR_ANY_INIT
                       : (struct in6_addr)IN6ADDR_LOOPBACK_INIT;
    } else if (inet6_pton(nodename, &addr_inet6, &scope_inet6) == 0) {
      have_inet6 = false;
    }
  }

  // Attempt to parse the service name.
  struct alias aliases[NALIASES_MAX];
  size_t naliases = 0;
#define ADD_ALIAS(v_socktype, v_protocol, v_port) \
  do {                                            \
    aliases[naliases].socktype = (v_socktype);    \
    aliases[naliases].protocol = (v_protocol);    \
    aliases[naliases++].port = (v_port);          \
  } while (0)
  if (servname == NULL) {
    // No service name provided. Return a single entry for SOCK_STREAM.
    ADD_ALIAS(SOCK_STREAM, IPPROTO_TCP, 0);
  } else {
    // Parse a decimal port number.
    bool have_port = true;
    const char *s = servname;
    in_port_t port = 0;
    do {
      if (*s < '0' || *s > '9' || mul_overflow(port, 10, &port) ||
          add_overflow(port, *s - '0', &port)) {
        have_port = false;
        break;
      }
    } while (*++s != '\0');

    if (have_port) {
      // Valid decimal port number.
      if (hints->ai_socktype == 0) {
        // A decimal port number does not allow us to derive the
        // protocol. Return one entry for TCP and one for UDP.
        ADD_ALIAS(SOCK_STREAM, IPPROTO_TCP, htons(port));
        ADD_ALIAS(SOCK_DGRAM, IPPROTO_UDP, htons(port));
      } else {
        ADD_ALIAS(hints->ai_socktype, hints->ai_protocol, htons(port));
      }
    } else if ((hints->ai_flags & AI_NUMERICSERV) == 0) {
      // String service name. Determine which protocols we want to return.
      // TODO(ed): Support SCTP and DCCP?
      bool want_tcp, want_udp;
      switch (hints->ai_socktype) {
        case 0:
          // Both TCP and UDP.
          want_tcp = true;
          want_udp = true;
          break;
        case SOCK_STREAM:
          // TCP socket.
          if (hints->ai_protocol != 0 && hints->ai_protocol != IPPROTO_TCP)
            return EAI_SOCKTYPE;
          want_tcp = true;
          want_udp = false;
          break;
        case SOCK_DGRAM:
          // UDP socket.
          if (hints->ai_protocol != 0 && hints->ai_protocol != IPPROTO_UDP)
            return EAI_SOCKTYPE;
          want_tcp = false;
          want_udp = true;
          break;
        default:
          return EAI_SOCKTYPE;
      }
      naliases = 0;
      for (const char *entry = __iana_port_numbers;
           portstr_get_port(entry) != 0; entry = portstr_get_next(entry)) {
        if (strcasecmp(portstr_get_name(entry), servname) == 0) {
          if (want_tcp && portstr_match_protoid(entry, PORTSTR_TCP))
            ADD_ALIAS(SOCK_STREAM, IPPROTO_TCP, portstr_get_port(entry));
          if (want_udp && portstr_match_protoid(entry, PORTSTR_UDP))
            ADD_ALIAS(SOCK_DGRAM, IPPROTO_UDP, portstr_get_port(entry));
        }
      }
    }
  }
#undef ADD_ALIAS

  // Allocate space for the return value. Initialize the entries by
  // linking them together.
  size_t nentries = ((size_t)have_inet + (size_t)have_inet6) * naliases;
  if (nentries == 0)
    return EAI_NONAME;
  struct entry *entries = calloc(nentries, sizeof(struct entry));
  if (entries == NULL)
    return EAI_MEMORY;
  for (size_t i = 0; i < nentries; ++i) {
    struct addrinfo *ai = &entries[i].ai;
    ai->ai_addr = &entries[i].addr.sa;
    ai->ai_canonname = (char *)nodename;
    if (i < nentries - 1)
      ai->ai_next = &entries[i + 1].ai;
  }

  // First return the IPv6 address entries.
  struct entry *entry = entries;
  if (have_inet6) {
    for (size_t i = 0; i < naliases; ++i) {
      const struct alias *alias = &aliases[i];
      struct addrinfo *ai = &entry->ai;
      ai->ai_family = AF_INET6;
      ai->ai_socktype = alias->socktype;
      ai->ai_protocol = alias->protocol;
      ai->ai_addrlen = sizeof(struct sockaddr_in6);

      struct sockaddr_in6 *sin6 = &entry->addr.sin6;
      sin6->sin6_family = AF_INET6;
      sin6->sin6_port = alias->port;
      sin6->sin6_addr = addr_inet6;
      sin6->sin6_scope_id = scope_inet6;
      ++entry;
    }
  }

  // Then return the IPv4 address entries.
  if (have_inet) {
    for (size_t i = 0; i < naliases; ++i) {
      const struct alias *alias = &aliases[i];
      struct addrinfo *ai = &entry->ai;
      ai->ai_family = AF_INET;
      ai->ai_socktype = alias->socktype;
      ai->ai_protocol = alias->protocol;
      ai->ai_addrlen = sizeof(struct sockaddr_in);

      struct sockaddr_in *sin = &entry->addr.sin;
      sin->sin_family = AF_INET;
      sin->sin_port = alias->port;
      sin->sin_addr = addr_inet;
      ++entry;
    }
  }

  *res = &entries[0].ai;
  return 0;
}
