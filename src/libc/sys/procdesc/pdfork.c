// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/arc4random.h>
#include <common/crt.h>
#include <common/program_getuuid.h>
#include <common/pthread.h>
#include <common/refcount.h>

#include <sys/procdesc.h>

#include <cloudabi_syscalls.h>
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
    // TODO(ed): We should skip pshared locks.
    refcount_init(&__pthread_num_threads, 1);
    __pthread_thread_id = tid;
    __pthread_lock_t *lock;
    atomic_init(&__pthread_self_object->join, tid | CLOUDABI_LOCK_WRLOCKED);
    refcount_init(&__pthread_self_object->refcount, 2);
    LIST_FOREACH(lock, &__pthread_wrlocks, __write_locks) {
      atomic_init(&lock->__state, tid | CLOUDABI_LOCK_WRLOCKED);
    }

    // Deplete the PRNG pool for this new thread. This will cause it to
    // refresh its state the next time we need random data.
    __arc4random_bytesleft = 0;

    // Force the per-process UUID to be regenerated.
    __program_getuuid_init = (pthread_once_t)PTHREAD_ONCE_INIT;

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
