// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <sys/socket.h>

#include <arpa/inet.h>

#include <netinet/in.h>

#include <assert.h>
#include <netdb.h>
#include <string.h>

static_assert(NI_MAXHOST >= INET_ADDRSTRLEN,
              "NI_MAXHOST too small to fit an IPv4 address");
static_assert(NI_MAXHOST >= INET6_ADDRSTRLEN + 11,
              "NI_MAXHOST too small to fit an IPv6 address and scope ID");

int getnameinfo(const struct sockaddr *restrict sa, size_t salen,
                char *restrict node, size_t nodelen, char *restrict service,
                size_t servicelen, int flags) {
  // Validate flags.
  if ((flags & ~(NI_NUMERICSERV | NI_DGRAM)) != 0)
    return EAI_BADFLAGS;
  if (node == NULL && service == NULL)
    return EAI_NONAME;

  // Call inet_ntop() to convert the address.
  in_port_t port;
  switch (sa->sa_family) {
    case AF_INET: {
      if (salen < sizeof(struct sockaddr_in))
        return EAI_FAMILY;
      const struct sockaddr_in *sin = (const struct sockaddr_in *)sa;
      if (node != NULL) {
        if (inet_ntop(AF_INET, &sin->sin_addr, node, nodelen) == NULL)
          return EAI_OVERFLOW;
      }
      port = ntohs(sin->sin_port);
      break;
    }
    case AF_INET6: {
      if (salen < sizeof(struct sockaddr_in6))
        return EAI_FAMILY;
      const struct sockaddr_in6 *sin6 = (const struct sockaddr_in6 *)sa;
      if (node != NULL) {
        if (inet_ntop(AF_INET6, &sin6->sin6_addr, node, nodelen) == NULL)
          return EAI_OVERFLOW;
        // TODO(ed): Scope!
      }
      port = ntohs(sin6->sin6_port);
      break;
    }
    default:
      return EAI_FAMILY;
  }

  if (service != NULL) {
    if ((flags & NI_NUMERICSERV) == 0) {
      // Textual service name representation.
      // TODO(ed): Implement this!
      return EAI_SYSTEM;
    } else {
      // Numerical service name representation.
      char portbuf[6];
      char *portstr = portbuf + sizeof(portbuf);
      *--portstr = '\0';
      do {
        *--portstr = port % 10 + '0';
        port /= 10;
      } while (port != 0);
      size_t portstrlen = portbuf + sizeof(portbuf) - portstr;
      if (portstrlen > servicelen)
        return EAI_OVERFLOW;
      memcpy(service, portstr, portstrlen);
    }
  }
  return 0;
}
