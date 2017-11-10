// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/pthread.h>

#include <errno.h>
#include <pthread.h>
#include <stdlib.h>

static void do_nothing(void *value) {
}

int pthread_key_create(pthread_key_t *key, void (*destructor)(void *)) {
  struct __pthread_key *keyobj;
  pthread_mutex_lock(&__pthread_key_freelist_lock);
  if (SLIST_EMPTY(&__pthread_key_freelist)) {
    // Freelist is empty. Allocate new key object.
    pthread_mutex_unlock(&__pthread_key_freelist_lock);
    keyobj = malloc(sizeof(*key->__key));
    if (keyobj == NULL)
      return ENOMEM;
    keyobj->generation = 0;
  } else {
    // Pick a key object from the freelist.
    keyobj = SLIST_FIRST(&__pthread_key_freelist);
    SLIST_REMOVE_HEAD(&__pthread_key_freelist, freelist);
    pthread_mutex_unlock(&__pthread_key_freelist_lock);
  }

  // Initialize key object.
  keyobj->destructor = destructor != NULL ? destructor : do_nothing;
  key->__key = keyobj;
  return 0;
}
