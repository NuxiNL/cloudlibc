// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/crt.h>
#include <common/pthread.h>
#include <common/refcount.h>
#include <common/syscalls.h>

#include <sys/procdesc.h>

#include <errno.h>
#include <stdatomic.h>

int pdfork(int *fdp, ...) {
  // Invoke "prepare" fork handlers in inverse order. Invoke the last
  // registered fork handler first.
  struct pthread_atfork *last =
      atomic_load_explicit(&__pthread_atfork_last, memory_order_relaxed);
  struct pthread_atfork *first = last;
  if (first != NULL) {
    for (;;) {
      if (first->prepare != NULL)
        first->prepare();
      if (first->previous == NULL)
        break;

      // Patch up next pointer of the previous element, as they cannot
      // be set by pthread_atfork() safely. These pointers are used
      // later on to iterate through the list forward.
      first->previous->next = first;
      first = first->previous;
    }
  }

  // Execute fork system call.
  cloudabi_fd_t fd;
  cloudabi_tid_t tid;
  cloudabi_errno_t error = cloudabi_sys_proc_fork(&fd, &tid);
  if (error != 0) {
    errno = error;
    return -1;
  }

  if (fd == CLOUDABI_PROCESS_CHILD) {
    // We're the child. Update our thread ID and patch up all locks held
    // by this thread to use the proper thread ID and to have no waiters.
    // TODO(edje): We should skip pshared locks.
    refcount_init(&__pthread_num_threads, 1);
    __pthread_thread_id = tid;
    __pthread_lock_t *lock;
    atomic_init(&__pthread_self_object->join, tid | CLOUDABI_LOCK_WRLOCKED);
    atomic_init(&__pthread_self_object->detachstate, 0);
    LIST_FOREACH(lock, &__pthread_wrlocks, __write_locks) {
      atomic_init(&lock->__state, tid | CLOUDABI_LOCK_WRLOCKED);
    }

    // Invoke "child" fork handlers.
    if (first != NULL) {
      for (;;) {
        if (first->child != NULL)
          first->child();
        if (first == last)
          break;
        first = first->next;
      }
    }

    return 0;
  } else {
    // We're the parent. Invoke "parent" fork handlers.
    if (first != NULL) {
      for (;;) {
        if (first->parent != NULL)
          first->parent();
        if (first == last)
          break;
        first = first->next;
      }
    }

    // Normally this would return the process ID, but this information
    // is not available in this environment.
    *fdp = fd;
    return 1;
  }
}
