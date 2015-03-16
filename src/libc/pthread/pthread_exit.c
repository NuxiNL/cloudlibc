// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/crt.h>
#include <common/pthread.h>
#include <common/refcount.h>
#include <common/syscalls.h>

#include <assert.h>
#include <limits.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdlib.h>

// Integration with jemalloc.
void _malloc_thread_cleanup(void);

noreturn void pthread_exit(void *value_ptr) {
  // Invoke cleanup routines registered by pthread_cleanup_push().
  const struct __pthread_cleanup *cleanup = __pthread_cleanup_stack;
  __pthread_cleanup_stack = NULL;
  while (cleanup != NULL) {
    cleanup->__routine(cleanup->__arg);
    cleanup = cleanup->__prev;
  }

  // Invoke destructors for per-thread values. These destructor functions
  // can register new values, so perform this step multiple times.
  //
  // TODO(edje): Is it valid to reset all values to NULL at once? This
  // could be solved more elegantly if we used a splay tree, as we could
  // just perform lookups while increasing the address.
  for (int i = 0;
       i < PTHREAD_DESTRUCTOR_ITERATIONS && !SLIST_EMPTY(&__pthread_specifics);
       ++i) {
    struct pthread_specifics list = SLIST_HEAD_INITIALIZER(list);
    SLIST_SWAP(&__pthread_specifics, &list, pthread_specific);
    struct pthread_specific *specific, *specific_tmp;
    SLIST_FOREACH_SAFE(specific, &list, next, specific_tmp) {
      if (specific->key->generation == specific->generation)
        specific->key->destructor(specific->value);
      free(specific);
    }
  }
  assert(SLIST_EMPTY(&__pthread_specifics) &&
         "Failed to destroy all per-thread values within "
         "PTHREAD_DESTRUCTOR_ITERATIONS iterations");

  // After invoking the cleanup functions and destructors, we should not
  // have picked up any locks.
  assert(LIST_EMPTY(&__pthread_wrlocks) &&
         "Attempted to terminate thread while holding write locks");
  assert(__pthread_rdlocks == 0 &&
         "Attempted to terminate thread while holding read locks");

  // Instead of terminating the thread, terminate the process regularly
  // if we're the last remaining thread.
  if (refcount_release(&__pthread_num_threads) == 1)
    exit(0);

  // Deinitialize malloc().
  _malloc_thread_cleanup();

  // In case the thread has an automatically allocated stack and has
  // been detached, we need to enqueue its deallocation.
  pthread_t self = __pthread_self_object;
  if (self->buffer != NULL) {
    unsigned int old = atomic_fetch_or_explicit(
        &self->detachstate, DETACH_TERMINATING, memory_order_relaxed);
    assert((old & DETACH_TERMINATING) == 0 &&
           "Attempted to terminate thread twice");
    if ((old & DETACH_DETACHED) != 0) {
      // TODO(edje): Implement.
    }
  }

  // Store the return value and terminate execution of the thread.
  self->return_value = value_ptr;
  cloudabi_sys_thread_exit(&self->join);
}
