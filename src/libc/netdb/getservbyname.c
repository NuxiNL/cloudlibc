// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <assert.h>
#include <netdb.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdlib.h>
#include <strings.h>

#include "netdb_impl.h"

// Conversion between protocol names and numbers.

static uint8_t proto_get_id(const char *name) {
  if (strcasecmp(name, "tcp") == 0)
    return PORTSTR_TCP;
  if (strcasecmp(name, "udp") == 0)
    return PORTSTR_UDP;
  if (strcasecmp(name, "sctp") == 0)
    return PORTSTR_SCTP;
  if (strcasecmp(name, "dccp") == 0)
    return PORTSTR_DCCP;
  return 0;
}

static const char *proto_get_name(uint8_t protoid) {
  switch (protoid) {
    case PORTSTR_TCP:
      return "tcp";
    case PORTSTR_UDP:
      return "udp";
    case PORTSTR_SCTP:
      return "sctp";
    case PORTSTR_DCCP:
      return "dccp";
  }
  assert(0 && "Invalid protocol ID");
  return NULL;
}

struct servent_entry {
  const char *entry;
  uint8_t protoid;
  struct servent servent;
  struct servent_entry *next;
};

// Allocates a new struct servent object based on an entry from the IANA
// port number registry.
static struct servent_entry *create_servent_entry(const char *entry,
                                                  uint8_t protoid) {
  // Compute the number of aliases this entry has.
  size_t aliases_count = 1;
  for (const char *alias = portstr_get_next(entry);
       portstr_get_port(alias) == portstr_get_port(entry);
       alias = portstr_get_next(alias)) {
    if (portstr_match_protoid(alias, protoid))
      ++aliases_count;
  }

  // Allocate space to allocate the entry. We'll store the servent
  // object, followed by the pointers to the strings.
  struct servent_entry *se =
      malloc(sizeof(*se) + aliases_count * sizeof(char *));
  if (se == NULL)
    return NULL;
  *se = (struct servent_entry){
      .entry = entry,
      .protoid = protoid,
      .servent =
          {
              .s_name = (char *)portstr_get_name(entry),
              .s_aliases = (char **)(se + 1),
              .s_port = portstr_get_port(entry),
              .s_proto = (char *)proto_get_name(protoid),
          },
  };

  // Fill the array with aliases.
  const char **aliases = (const char **)(se + 1);
  for (const char *alias = portstr_get_next(entry);
       portstr_get_port(alias) == portstr_get_port(entry);
       alias = portstr_get_next(alias)) {
    if (portstr_match_protoid(alias, protoid))
      *aliases++ = portstr_get_name(alias);
  }
  *aliases = NULL;

  return se;
}

// Attempts to fetch a memoized copy of struct servent. If no copy is
// available, it creates a new copy through create_servent().
static struct servent *get_servent(const char *entry, uint8_t protoid) {
  static _Atomic(struct servent_entry *) table = ATOMIC_VAR_INIT(NULL);

  // Search for an existing copy.
  struct servent_entry *first =
      atomic_load_explicit(&table, memory_order_relaxed);
  for (struct servent_entry *se = first; se != NULL; se = se->next)
    if (se->entry == entry && se->protoid == protoid)
      return &se->servent;

  // Existing entry not found. Allocate a new entry.
  struct servent_entry *se_new = create_servent_entry(entry, protoid);
  if (se_new == NULL)
    return NULL;

  for (;;) {
    // Store the new entry in the global list. The list is lockless, so
    // perform a compare-and-exchange.
    se_new->next = first;
    if (atomic_compare_exchange_weak(&table, &first, se_new))
      return &se_new->servent;

    // Compare-and-exchange failed. See if a servent for the same entry
    // got created in the meantime.
    for (struct servent_entry *se = first; se != se_new->next; se = se->next)
      if (se->entry == entry && se->protoid == protoid) {
        free(se_new);
        return &se->servent;
      }
  }
}

// Public API.

// Returns the first entry that matches both the name (or one of the
// aliases) and the protocol.
struct servent *getservbyname(const char *name, const char *proto) {
  uint8_t protoid = proto_get_id(proto);
  const char *first_entry = __iana_port_numbers;
  for (const char *entry = __iana_port_numbers; portstr_get_port(entry) != 0;
       entry = portstr_get_next(entry)) {
    if (portstr_match_protoid(entry, protoid)) {
      // Keep track of the first entry that has the same port number and
      // matches the protocol, as this data is used by create_servent().
      if (portstr_get_port(first_entry) != portstr_get_port(entry))
        first_entry = entry;

      // Match against the name.
      if (strcasecmp(portstr_get_name(entry), name) == 0)
        return get_servent(first_entry, protoid);
    }
  }
  return NULL;
}

// Returns the first entry that matches both the port number and the
// protocol.
struct servent *getservbyport(int port, const char *proto) {
  uint8_t protoid = proto_get_id(proto);
  for (const char *entry = __iana_port_numbers; portstr_get_port(entry) != 0;
       entry = portstr_get_next(entry)) {
    if (portstr_get_port(entry) == port &&
        portstr_match_protoid(entry, protoid))
      return get_servent(entry, protoid);
  }
  return NULL;
}
