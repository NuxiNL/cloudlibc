// Copyright (c) 2015-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#ifndef COMMON_PTHREAD_H
#define COMMON_PTHREAD_H

#include <common/queue.h>
#include <common/refcount.h>

#include <assert.h>
#include <cloudabi_types.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdnoreturn.h>
#include <threads.h>

// On Linux, the default stack size for new threads is 2 MB. On FreeBSD
// it is either 1 MB for 32-bit systems and 2 MB for 64-bit systems.
// As this implement requires two stacks due to SafeStack, allocate two
// stacks that are both 1 MB in size.
#define PTHREAD_STACK_DEFAULT (1 << 20)

// Alignment of the unsafe stack buffer.
#define PTHREAD_UNSAFE_STACK_ALIGNMENT 16
static_assert(PTHREAD_STACK_DEFAULT % PTHREAD_UNSAFE_STACK_ALIGNMENT == 0,
              "Default stack buffer needs to be properly aligned");

// Number of threads currently active. This counter is used by
// pthread_exit() to determine whether the process should be terminated
// gracefully if the number of threads would reach zero.
extern refcount_t __pthread_num_threads;

// Keys.
//
// This implementation of pthread keys is written in such a way that no
// locking is required to get and set key values, but also during thread
// shutdown.
//
// Key objects are never deallocated, but are reused. They each have an
// individual generation counter. Threads also store a copy of the
// generation counter when referring to the key object, meaning they can
// determine whether the destructor function still corresponds with the
// value of the key for that thread.

struct __pthread_key {
  SLIST_ENTRY(__pthread_key) freelist;
  uint_least64_t generation;
  void (*destructor)(void *);
};

struct pthread_specific {
  SLIST_ENTRY(pthread_specific) next;
  struct __pthread_key *key;
  void *value;
  uint_least64_t generation;
};

// Key freelist.
extern pthread_mutex_t __pthread_key_freelist_lock;
extern SLIST_HEAD(pthread_key_freelist, __pthread_key) __pthread_key_freelist
    __guarded_by(__pthread_key_freelist_lock);

// Per-thread specific value list.
//
// TODO(ed): Use a splay tree instead?
extern thread_local SLIST_HEAD(pthread_specifics,
                               pthread_specific) __pthread_specifics;

// Locking.
//
// As this implementation of POSIX threading implements rwlocks in such
// a way that they would not impose any overhead in comparison to a
// plain mutex, mutexes are simply implemented on top of rwlocks. There
// is only one kind of lock: rwlocks.
//
// The rwlocks provided by this implementation prevent writer starvation
// by preferring write locks over read locks. This would become
// problematic if a thread would try to recursively read-lock a lock
// with waiting writers, as this would cause a deadlock.
//
// To mitigate this, threads already that hold a read lock are allowed
// to ignore this behaviour. Ideally we should keep track of an exact
// list of read locks held by a thread and only allow this to happen on
// those locks, but that would require too much storage space. Use a
// simple counter instead.

// Number of times locking in userspace should be attempted before
// calling into the kernel.
// TODO(ed): Determine the right number!
#define LOCK_RETRY_COUNT 1

// The number of read locks acquired. This is used by
// pthread_rwlock_rdlock() to determine whether to ignore waiting
// writers.
extern thread_local unsigned int __pthread_rdlocks;

// Terminate the process due to a locking error.
noreturn void __pthread_terminate(cloudabi_errno_t, const char *);

struct thread_atexit {
  // Handler registered by __cxa_thread_atexit().
  void (*func)(void *);
  void *arg;

  // Previous list element.
  struct thread_atexit *previous;
};

// Pointer to last __cxa_thread_atexit() handler.
extern _Atomic(struct thread_atexit *) __thread_atexit_last;

#endif
