// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/crt.h>
#include <common/pthread.h>
#include <common/refcount.h>

#include <cloudabi_syscalls.h>
#include <errno.h>
#include <pthread.h>
#include <stdalign.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>

static noreturn void thread_entry(cloudabi_tid_t tid, void *data) {
  // Set up TLS space.
  char tls_space[__pt_tls_memsz_aligned + __pt_tls_align - 1];
  char *tls_start = (char *)__roundup((uintptr_t)tls_space, __pt_tls_align);
  memcpy(tls_start, __pt_tls_vaddr_abs, __pt_tls_filesz);
  memset(tls_start + __pt_tls_filesz, '\0',
         __pt_tls_memsz_aligned - __pt_tls_filesz);
#if defined(__aarch64__)
  asm volatile("msr tpidr_el0, %0" : : "r"(tls_start - 16));
#elif defined(__x86_64__)
  char *tls_end = tls_start + __pt_tls_memsz_aligned;
  cloudabi_sys_thread_tcb_set(&tls_end);
#else
#error "Unsupported architecture"
#endif

  // Fix up some of the variables stored in TLS.
  pthread_t handle = data;
  __pthread_self_object = handle;
  __pthread_thread_id = tid;
  __safestack_unsafe_stack_ptr = (void *)__rounddown(
      (uintptr_t)handle->unsafe_stack + handle->unsafe_stacksize,
      PTHREAD_UNSAFE_STACK_ALIGNMENT);

  // Initialize the the once object used for joining. This is also done
  // by the parent, but it may be the case that we call pthread_exit()
  // before the parent has a chance to initialize it.
  cloudabi_lock_t expected = CLOUDABI_LOCK_BOGUS;
  atomic_compare_exchange_strong_explicit(
      &handle->join, &expected, tid | CLOUDABI_LOCK_WRLOCKED,
      memory_order_acquire, memory_order_relaxed);

  // Free thread startup information and invoke user-supplied start routine.
  void *(*start_routine)(void *) = handle->start_routine;
  void *argument = handle->argument;
  pthread_exit(start_routine(argument));
}

int pthread_create(pthread_t *restrict thread,
                   const pthread_attr_t *restrict attr,
                   void *(*start_routine)(void *), void *restrict arg) {
  size_t stacksize = attr != NULL ? attr->__stacksize : PTHREAD_STACK_DEFAULT;
  size_t safe_stacksize =
      stacksize + __pt_tls_memsz_aligned + sizeof(struct __pthread);
  char *safe_stack = malloc(safe_stacksize);
  if (safe_stack == NULL)
    return EAGAIN;
  size_t unsafe_stacksize = stacksize;
  char *unsafe_stack = malloc(unsafe_stacksize);
  if (unsafe_stack == NULL) {
    free(safe_stack);
    return EAGAIN;
  }

  // Steal a part from the top of the stack to store the thread handle.
  pthread_t handle = (pthread_t)__rounddown(
      (uintptr_t)safe_stack + safe_stacksize - sizeof(struct __pthread),
      alignof(struct __pthread));
  safe_stacksize = (char *)handle - safe_stack;
  *handle = (struct __pthread){
      .join = ATOMIC_VAR_INIT(CLOUDABI_LOCK_BOGUS),
      .safe_stack = safe_stack,
      .unsafe_stack = unsafe_stack,
      .unsafe_stacksize = unsafe_stacksize,

      .detachstate = ATOMIC_VAR_INIT(
          attr != NULL && attr->__detachstate == PTHREAD_CREATE_DETACHED
              ? DETACH_DETACHED
              : 0),

      .start_routine = start_routine,
      .argument = arg,
  };

  // Bump up the number of threads currently active.
  refcount_acquire(&__pthread_num_threads);

  // Create thread.
  cloudabi_threadattr_t tdattr = {
      .entry_point = thread_entry,
      .stack = safe_stack,
      .stack_size = safe_stacksize,
      .argument = handle,
  };
  cloudabi_tid_t tid;
  cloudabi_errno_t error = cloudabi_sys_thread_create(&tdattr, &tid);
  if (error != 0) {
    refcount_release(&__pthread_num_threads);
    free(safe_stack);
    free(unsafe_stack);
    return error;
  }

  // Now that the thread ID is known, we can initialize the lock object
  // used for joining.
  cloudabi_lock_t expected = CLOUDABI_LOCK_BOGUS;
  atomic_compare_exchange_strong_explicit(
      &handle->join, &expected, tid | CLOUDABI_LOCK_WRLOCKED,
      memory_order_acquire, memory_order_relaxed);

  // Return the thread handle.
  *thread = handle;
  return 0;
}
