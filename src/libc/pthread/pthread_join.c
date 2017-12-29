// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/crt.h>
#include <common/pthread.h>
#include <common/refcount.h>

#include <assert.h>
#include <cloudabi_syscalls.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>

int pthread_join(pthread_t thread, void **value_ptr) {
  assert(thread != __pthread_self_object &&
         "Thread attempted to join with itself");
  cloudabi_lock_t old =
      atomic_load_explicit(&thread->join, memory_order_acquire);
  if ((old & CLOUDABI_LOCK_WRLOCKED) != 0) {
    assert((old & CLOUDABI_LOCK_KERNEL_MANAGED) == 0 &&
           "Corrupt pthread_t handle");

    // Thread is still running. Suspend execution of this thread until
    // the other thread has terminated. Blocking is performed by
    // attempting to acquire a read lock on a lock that is owned by the
    // other thread. This allows the kernel to apply priority
    // inheritance.
    cloudabi_subscription_t subscription = {
        .type = CLOUDABI_EVENTTYPE_LOCK_RDLOCK,
        .lock.lock = &thread->join,
        .lock.lock_scope = CLOUDABI_SCOPE_PRIVATE,
    };
    size_t triggered;
    cloudabi_event_t event;
    cloudabi_errno_t error =
        cloudabi_sys_poll(&subscription, &event, 1, &triggered);
    if (error != 0)
      __pthread_terminate(error, "Failed to join on thread");
    if (event.error != 0)
      __pthread_terminate(event.error, "Failed to join on thread");
  }

  // Store thread return value.
  if (value_ptr != NULL)
    *value_ptr = thread->return_value;

  // Free the stack buffers associated with this thread. The safe stack
  // buffer also contains the thread's handle.
  refcount_assert_exclusive(&thread->refcount);
  void *unsafe_stack = thread->unsafe_stack;
  free(thread->safe_stack);
  free(unsafe_stack);
  return 0;
}
