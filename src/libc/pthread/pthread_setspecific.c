// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/pthread.h>

#include <errno.h>
#include <pthread.h>
#include <stdlib.h>

int pthread_setspecific(pthread_key_t key, const void *value) {
  struct __pthread_key *keyobj = key.__key;
  if (value == NULL) {
    // Clear value associated with key.
    struct pthread_specific *specific, **specific_prev;
    SLIST_FOREACH_PREVPTR(specific, specific_prev, &__pthread_specifics, next) {
      if (specific->key == keyobj) {
        // Remove entry.
        SLIST_REMOVE_PREVPTR(specific_prev, next);
        free(specific);
        return 0;
      } else if (specific->generation != specific->key->generation) {
        // Garbage collect entries whose key has been destroyed in the meantime.
        SLIST_REMOVE_PREVPTR(specific_prev, next);
        free(specific);
      }
    }
  } else {
    // Set value associated with key.
    struct pthread_specific *specific, **specific_prev;
    SLIST_FOREACH_PREVPTR(specific, specific_prev, &__pthread_specifics, next) {
      if (specific->key == keyobj) {
        // Update entry. Move the entry to the head of the list to speed
        // up future requests.
        specific->generation = keyobj->generation;
        specific->value = (void *)value;
        SLIST_REMOVE_PREVPTR(specific_prev, next);
        SLIST_INSERT_HEAD(&__pthread_specifics, specific, next);
        return 0;
      } else if (specific->generation != specific->key->generation) {
        // Garbage collect entries whose key has been destroyed in the meantime.
        SLIST_REMOVE_PREVPTR(specific_prev, next);
        free(specific);
      }
    }

    // No match found. Allocate new entry and place new entry in the list.
    struct pthread_specific *new = malloc(sizeof(*new));
    if (new == NULL)
      return ENOMEM;
    new->key = keyobj;
    new->generation = keyobj->generation;
    new->value = (void *)value;
    SLIST_INSERT_HEAD(&__pthread_specifics, new, next);
  }
  return 0;
}
