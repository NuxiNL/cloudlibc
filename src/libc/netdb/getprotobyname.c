// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <netdb.h>
#include <stddef.h>
#include <strings.h>

#include "iana_protocol_numbers.h"

// Returns the first entry from the protocol database having the same
// name or having an alias with the same name.
struct protoent *getprotobyname(const char *name) {
  for (size_t i = 0; i < __arraycount(entries); ++i) {
    const struct protoent *pe = &entries[i];

    // Compare the name.
    if (strcasecmp(pe->p_name, name) == 0)
      return (struct protoent *)pe;

    // Compare the aliases.
    for (char **alias = pe->p_aliases; *alias != NULL; ++alias) {
      if (strcasecmp(*alias, name) == 0)
        return (struct protoent *)pe;
    }
  }
  return NULL;
}

// Returns the first entry from the protocol database having the same
// protocol number.
struct protoent *getprotobynumber(int proto) {
  for (size_t i = 0; i < __arraycount(entries); ++i) {
    const struct protoent *pe = &entries[i];

    // Compare the protocol number.
    if (pe->p_proto == proto)
      return (struct protoent *)pe;
  }
  return NULL;
}
