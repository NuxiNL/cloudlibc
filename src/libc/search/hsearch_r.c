// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <errno.h>
#include <limits.h>
#include <search.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "search_impl.h"

// Look up an unused entry in the hash table for a given hash. For this
// implementation we use quadratic probing. Quadratic probing has the
// advantage of preventing primary clustering.
static ENTRY *hsearch_lookup_free(struct __hsearch *hsearch, size_t hash) {
  for (size_t index = hash, i = 0;; index += ++i) {
    ENTRY *entry = &hsearch->entries[index & hsearch->index_mask];
    if (entry->key == NULL)
      return entry;
  }
}

// Computes an FNV-1a hash of the key. Depending on the pointer size, this
// either uses the 32- or 64-bit FNV prime.
static size_t hsearch_hash(size_t offset_basis, const char *str) {
  size_t hash = offset_basis;
  while (*str != '\0') {
    hash ^= (uint8_t)*str++;
    // clang-format off
    if (sizeof(size_t) * CHAR_BIT <= 32)
      hash *= UINT32_C(16777619);
    else
      hash *= UINT64_C(1099511628211);
    // clang-format on
  }
  return hash;
}

int hsearch_r(ENTRY item, ACTION action, ENTRY **retval,
              struct hsearch_data *htab) {
  struct __hsearch *hsearch = htab->__hsearch;
  size_t hash = hsearch_hash(hsearch->offset_basis, item.key);

  // Search the hash table for an existing entry for this key. Stop
  // searching if we run into an unused hash table entry.
  ENTRY *entry;
  for (size_t index = hash, i = 0;; index += ++i) {
    entry = &hsearch->entries[index & hsearch->index_mask];
    if (entry->key == NULL)
      break;
    if (strcmp(entry->key, item.key) == 0) {
      *retval = entry;
      return 1;
    }
  }

  // Only perform the insertion if action is set to ENTER.
  if (action == FIND) {
    errno = ESRCH;
    return 0;
  }

  if (hsearch->entries_used * 2 >= hsearch->index_mask) {
    // Preserve the old hash table entries.
    size_t old_count = hsearch->index_mask + 1;
    ENTRY *old_entries = hsearch->entries;

    // Allocate and install a new table if insertion would yield a hash
    // table that is more than 50% used. By using 50% as a threshold, a
    // lookup will only take up to two steps on average.
    size_t new_count = (hsearch->index_mask + 1) * 2;
    ENTRY *new_entries = calloc(new_count, sizeof(ENTRY));
    if (new_entries == NULL)
      return 0;
    hsearch->entries = new_entries;
    hsearch->index_mask = new_count - 1;

    // Copy over the entries from the old table to the new table.
    for (size_t i = 0; i < old_count; ++i) {
      const ENTRY *old_entry = &old_entries[i];
      if (old_entry->key != NULL) {
        size_t old_hash = hsearch_hash(hsearch->offset_basis, old_entry->key);
        *hsearch_lookup_free(hsearch, old_hash) = *old_entry;
      }
    }

    // Destroy the old hash table entries.
    free(old_entries);

    // Perform a new lookup for a free table entry, so that we insert
    // the entry into the new hash table.
    hsearch = htab->__hsearch;
    entry = hsearch_lookup_free(hsearch, hash);
  }

  // Insert the new entry into the hash table.
  *entry = item;
  ++hsearch->entries_used;
  *retval = entry;
  return 1;
}
