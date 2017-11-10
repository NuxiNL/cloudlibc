// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/crt.h>
#include <common/pthread.h>
#include <common/refcount.h>

#include <assert.h>
#include <cloudabi_syscalls.h>
#include <limits.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdlib.h>

// Integration with jemalloc.
void __malloc_thread_cleanup(void);

noreturn void pthread_exit(void *value_ptr) {
  // Invoke cleanup routines registered by __cxa_thread_atexit().
  for (struct thread_atexit *entry =
           atomic_load_explicit(&__thread_atexit_last, memory_order_relaxed);
       entry != NULL; entry = entry->previous)
    entry->func(entry->arg);

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
  // TODO(ed): Is it valid to reset all values to NULL at once? This
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
  assert(__pthread_rdlocks == 0 &&
         "Attempted to terminate thread while holding read locks");

  // Instead of terminating the thread, terminate the process regularly
  // if we're the last remaining thread. Prevent calling the
  // __cxa_thread_exit() cleanup routines twice.
  if (refcount_release(&__pthread_num_threads)) {
    atomic_store_explicit(&__thread_atexit_last, NULL, memory_order_relaxed);
    exit(0);
  }

  // In case the thread has been detached, we need to ensure some other
  // thread deallocates our stack. Simply store the thread handle in a
  // global variable, so that the next call to pthread_exit() of a
  // detached thread cleans it up. That way, a process will only 'leak'
  // up to one stack.
  pthread_t self = __pthread_self_object;
  if (refcount_release(&self->refcount)) {
    static struct __pthread dead_thread = {};
    static _Atomic(pthread_t) previous_detached_thread =
        ATOMIC_VAR_INIT(&dead_thread);
    pthread_t previous = atomic_exchange_explicit(&previous_detached_thread,
                                                  self, memory_order_relaxed);
    pthread_join(previous, NULL);
  }

  // Deinitialize malloc().
  __malloc_thread_cleanup();

  // Store the return value and terminate execution of the thread.
  self->return_value = value_ptr;
  cloudabi_sys_thread_exit(&self->join, CLOUDABI_SCOPE_PRIVATE);
}
