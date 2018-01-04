// Copyright (c) 2015-2018 Nuxi, https://nuxi.nl/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.

// <pthread.h> - threads
//
// Extensions:
// - PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP:
//   Also present on Linux.
// - pthread_t:
//   An arithmetic type, as required by SUSv2.
// - pthread_mutex_t, pthread_rwlock_t and pthread_spinlock_t:
//   All implemented using the same underlying type. pthread_cond_*wait()
//   can be used in combination with any type of lock.
// - pthread_mutexattr_t and pthread_rwlockattr_t:
//   All implemented using the same underlying type.
// - pthread_cond_timedwait_relative_np():
//   Identical to pthread_cond_timedwait(), except that it uses a
//   relative timeout on the monotonic clock. Also present on macOS.
// - pthread_mutex_lock_pair_np():
//   Acquires two locks using a deadlock avoidance algorithm.
//
// Features missing:
// - PTHREAD_CANCEL_ASYNCHRONOUS, PTHREAD_CANCEL_ENABLE,
//   PTHREAD_CANCEL_DEFERRED, PTHREAD_CANCEL_DISABLE, PTHREAD_CANCELED,
//   pthread_cancel(), pthread_setcancelstate(), pthread_setcanceltype()
//   and pthread_testcancel():
//   Thread cancellation is not supported. Threads must be terminated
//   through pthread_exit() explicitly.
// - PTHREAD_MUTEX_ERRORCHECK, PTHREAD_MUTEX_ROBUST and
//   pthread_mutex_consistent():
//   Incorrect use of mutexes will cause processes to abort. No explicit
//   error handling is required.
// - PTHREAD_PRIO_NONE and PTHREAD_PRIO_PROTECT,
//   pthread_mutex_getprioceiling(), pthread_mutex_setprioceiling(),
//   pthread_mutexattr_getprioceiling() and pthread_mutexattr_setprioceiling():
//   Mutexes always use PTHREAD_PRIO_INHERIT.
// - pthread_attr_getguardsize() and pthread_attr_setguardsize():
//   Guard size cannot be configured.
// - pthread_attr_getschedparam() and pthread_attr_setschedparam():
//   Scheduler interaction not available.

#ifndef _PTHREAD_H_
#define _PTHREAD_H_

#include <_/limits.h>
#include <_/types.h>

#include <sched.h>
#include <time.h>

// Returned to one of the threads calling pthread_barrier_wait().
#define PTHREAD_BARRIER_SERIAL_THREAD (-1)

// For pthread_attr_setdetachstate().
#define PTHREAD_CREATE_DETACHED 1
#define PTHREAD_CREATE_JOINABLE 2

// For pthread_mutexattr_settype().
#define PTHREAD_MUTEX_DEFAULT PTHREAD_MUTEX_NORMAL
#define PTHREAD_MUTEX_NORMAL 1
#define PTHREAD_MUTEX_RECURSIVE 2

// For pthread_mutexattr_setrobust().
#define PTHREAD_MUTEX_STALLED 1

// For pthread_mutexattr_setprotocol().
#define PTHREAD_PRIO_INHERIT 1

// For pthread_*_setpshared().
#define PTHREAD_PROCESS_PRIVATE 0x4
#define PTHREAD_PROCESS_SHARED 0x8

// Compile-time initializers.
#define _PTHREAD_FUTEX_INITIALIZER(v) _UINT32_C(v)
#define PTHREAD_COND_INITIALIZER \
  { _PTHREAD_FUTEX_INITIALIZER(0), 3, PTHREAD_PROCESS_PRIVATE }
#define PTHREAD_MUTEX_INITIALIZER \
  { _PTHREAD_FUTEX_INITIALIZER(0), -1, PTHREAD_PROCESS_PRIVATE }
#define PTHREAD_ONCE_INIT \
  { _PTHREAD_FUTEX_INITIALIZER(0x80000000) }
#define PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP \
  { _PTHREAD_FUTEX_INITIALIZER(0), 0, PTHREAD_PROCESS_PRIVATE }
#define PTHREAD_RWLOCK_INITIALIZER \
  { _PTHREAD_FUTEX_INITIALIZER(0), -1, PTHREAD_PROCESS_PRIVATE }

#ifndef _PTHREAD_ATTR_T_DECLARED
typedef __pthread_attr_t pthread_attr_t;
#define _PTHREAD_ATTR_T_DECLARED
#endif
#ifndef _PTHREAD_BARRIER_T_DECLARED
typedef __pthread_barrier_t pthread_barrier_t;
#define _PTHREAD_BARRIER_T_DECLARED
#endif
#ifndef _PTHREAD_BARRIERATTR_T_DECLARED
typedef __pthread_barrierattr_t pthread_barrierattr_t;
#define _PTHREAD_BARRIERATTR_T_DECLARED
#endif
#ifndef _PTHREAD_COND_T_DECLARED
typedef __pthread_cond_t pthread_cond_t;
#define _PTHREAD_COND_T_DECLARED
#endif
#ifndef _PTHREAD_CONDATTR_T_DECLARED
typedef __pthread_condattr_t pthread_condattr_t;
#define _PTHREAD_CONDATTR_T_DECLARED
#endif
#ifndef _PTHREAD_KEY_T_DECLARED
typedef __pthread_key_t pthread_key_t;
#define _PTHREAD_KEY_T_DECLARED
#endif
#ifndef _PTHREAD_MUTEX_T_DECLARED
typedef __pthread_lock_t pthread_mutex_t;
#define _PTHREAD_MUTEX_T_DECLARED
#endif
#ifndef _PTHREAD_MUTEXATTR_T_DECLARED
typedef __pthread_lockattr_t pthread_mutexattr_t;
#define _PTHREAD_MUTEXATTR_T_DECLARED
#endif
#ifndef _PTHREAD_ONCE_T_DECLARED
typedef __pthread_once_t pthread_once_t;
#define _PTHREAD_ONCE_T_DECLARED
#endif
#ifndef _PTHREAD_RWLOCK_T_DECLARED
typedef __pthread_lock_t pthread_rwlock_t;
#define _PTHREAD_RWLOCK_T_DECLARED
#endif
#ifndef _PTHREAD_RWLOCKATTR_T_DECLARED
typedef __pthread_lockattr_t pthread_rwlockattr_t;
#define _PTHREAD_RWLOCKATTR_T_DECLARED
#endif
#ifndef _PTHREAD_SPINLOCK_T_DECLARED
typedef __pthread_lock_t pthread_spinlock_t;
#define _PTHREAD_SPINLOCK_T_DECLARED
#endif
#ifndef _PTHREAD_T_DECLARED
typedef __pthread_t pthread_t;
#define _PTHREAD_T_DECLARED
#endif

// pthread_cleanup_push() and pthread_cleanup_pop().
//
// Use a thread-local variable (__pthread_cleanup_stack) to store a
// stack of cleanup routines. This stack is used by pthread_exit() to
// call any cleanup functions that have not been popped.

struct __pthread_cleanup {
  const struct __pthread_cleanup *__prev;

  void (*__routine)(void *);
  void *__arg;
};

extern _Thread_local const struct __pthread_cleanup *__pthread_cleanup_stack;

// clang-format off
#define pthread_cleanup_push(routine, arg)               \
  {                                                      \
    const struct __pthread_cleanup __pthread_cleanup = { \
        __pthread_cleanup_stack, routine, arg};          \
    __pthread_cleanup_stack = &__pthread_cleanup;        \
    {
#define pthread_cleanup_pop(execute)                        \
    }                                                       \
    if (execute)                                            \
      __pthread_cleanup.__routine(__pthread_cleanup.__arg); \
    __pthread_cleanup_stack = __pthread_cleanup.__prev;     \
  }
// clang-format on

__BEGIN_DECLS
int pthread_atfork(void (*)(void), void (*)(void), void (*)(void));
int pthread_attr_destroy(pthread_attr_t *);
int pthread_attr_getdetachstate(const pthread_attr_t *, int *);
int pthread_attr_getstacksize(const pthread_attr_t *__restrict,
                              size_t *__restrict);
int pthread_attr_init(pthread_attr_t *);
int pthread_attr_setdetachstate(pthread_attr_t *, int);
int pthread_attr_setstacksize(pthread_attr_t *, size_t);
int pthread_barrier_destroy(pthread_barrier_t *);
int pthread_barrier_init(pthread_barrier_t *__restrict,
                         const pthread_barrierattr_t *__restrict, unsigned int);
int pthread_barrier_wait(pthread_barrier_t *);
int pthread_barrierattr_destroy(pthread_barrierattr_t *);
int pthread_barrierattr_getpshared(const pthread_barrierattr_t *__restrict,
                                   int *__restrict);
int pthread_barrierattr_init(pthread_barrierattr_t *);
int pthread_barrierattr_setpshared(pthread_barrierattr_t *, int);
int pthread_cond_broadcast(pthread_cond_t *);
int pthread_cond_destroy(pthread_cond_t *);
int pthread_cond_init(pthread_cond_t *__restrict,
                      const pthread_condattr_t *__restrict);
int pthread_cond_signal(pthread_cond_t *);
int pthread_cond_timedwait(pthread_cond_t *__restrict,
                           __pthread_lock_t *__restrict __lock,
                           const struct timespec *__restrict)
    __requires_exclusive(*__lock);
int pthread_cond_timedwait_relative_np(pthread_cond_t *__restrict,
                                       __pthread_lock_t *__restrict __lock,
                                       const struct timespec *__restrict)
    __requires_exclusive(*__lock);
int pthread_cond_wait(pthread_cond_t *__restrict,
                      __pthread_lock_t *__restrict __lock)
    __requires_exclusive(*__lock);
int pthread_condattr_destroy(pthread_condattr_t *);
int pthread_condattr_getclock(const pthread_condattr_t *__restrict,
                              clockid_t *__restrict);
int pthread_condattr_getpshared(const pthread_condattr_t *__restrict,
                                int *__restrict);
int pthread_condattr_init(pthread_condattr_t *);
int pthread_condattr_setclock(pthread_condattr_t *, clockid_t);
int pthread_condattr_setpshared(pthread_condattr_t *, int);
int pthread_create(pthread_t *__restrict, const pthread_attr_t *__restrict,
                   void *(*)(void *), void *__restrict);
int pthread_detach(pthread_t);
int pthread_equal(pthread_t, pthread_t);
_Noreturn void pthread_exit(void *);
void *pthread_getspecific(pthread_key_t);
int pthread_join(pthread_t, void **);
int pthread_key_create(pthread_key_t *, void (*)(void *));
int pthread_key_delete(pthread_key_t);
int pthread_mutex_destroy(pthread_mutex_t *__mutex)
    __requires_unlocked(*__mutex);
int pthread_mutex_init(pthread_mutex_t *__restrict __mutex,
                       const pthread_mutexattr_t *__restrict)
    __requires_unlocked(*__mutex);
int pthread_mutex_lock(pthread_mutex_t *__mutex) __locks_exclusive(*__mutex);
int pthread_mutex_lock_pair_np(pthread_mutex_t *__mutex1,
                               pthread_mutex_t *__mutex2)
    __locks_exclusive(*__mutex1, *__mutex2);
int pthread_mutex_timedlock(pthread_mutex_t *__restrict __mutex,
                            const struct timespec *__restrict)
    __trylocks_exclusive(0, *__mutex);
int pthread_mutex_trylock(pthread_mutex_t *__mutex)
    __trylocks_exclusive(0, *__mutex);
int pthread_mutex_unlock(pthread_mutex_t *__mutex) __unlocks(*__mutex);
int pthread_mutexattr_destroy(pthread_mutexattr_t *);
int pthread_mutexattr_getprotocol(const pthread_mutexattr_t *__restrict,
                                  int *__restrict);
int pthread_mutexattr_getpshared(const pthread_mutexattr_t *__restrict,
                                 int *__restrict);
int pthread_mutexattr_getrobust(const pthread_mutexattr_t *__restrict,
                                int *__restrict);
int pthread_mutexattr_gettype(const pthread_mutexattr_t *__restrict,
                              int *__restrict);
int pthread_mutexattr_init(pthread_mutexattr_t *);
int pthread_mutexattr_setprotocol(pthread_mutexattr_t *, int);
int pthread_mutexattr_setpshared(pthread_mutexattr_t *, int);
int pthread_mutexattr_setrobust(pthread_mutexattr_t *, int);
int pthread_mutexattr_settype(pthread_mutexattr_t *, int);
int pthread_once(pthread_once_t *, void (*)(void));
int pthread_rwlock_destroy(pthread_rwlock_t *__rwlock)
    __requires_unlocked(*__rwlock);
int pthread_rwlock_init(pthread_rwlock_t *__restrict __rwlock,
                        const pthread_rwlockattr_t *__restrict)
    __requires_unlocked(*__rwlock);
int pthread_rwlock_rdlock(pthread_rwlock_t *__rwlock) __locks_shared(*__rwlock);
int pthread_rwlock_timedrdlock(pthread_rwlock_t *__restrict __rwlock,
                               const struct timespec *__restrict)
    __trylocks_shared(0, *__rwlock);
int pthread_rwlock_timedwrlock(pthread_rwlock_t *__restrict __rwlock,
                               const struct timespec *__restrict)
    __trylocks_exclusive(0, *__rwlock);
int pthread_rwlock_tryrdlock(pthread_rwlock_t *__rwlock)
    __trylocks_shared(0, *__rwlock);
int pthread_rwlock_trywrlock(pthread_rwlock_t *__rwlock)
    __trylocks_exclusive(0, *__rwlock);
int pthread_rwlock_unlock(pthread_rwlock_t *__rwlock) __unlocks(*__rwlock);
int pthread_rwlock_wrlock(pthread_rwlock_t *__rwlock)
    __locks_exclusive(*__rwlock);
int pthread_rwlockattr_destroy(pthread_rwlockattr_t *);
int pthread_rwlockattr_getpshared(const pthread_rwlockattr_t *__restrict,
                                  int *__restrict);
int pthread_rwlockattr_init(pthread_rwlockattr_t *);
int pthread_rwlockattr_setpshared(pthread_rwlockattr_t *, int);
pthread_t pthread_self(void);
int pthread_setspecific(pthread_key_t, const void *);
int pthread_spin_destroy(pthread_spinlock_t *__lock)
    __requires_unlocked(*__lock);
int pthread_spin_init(pthread_spinlock_t *__lock, int)
    __requires_unlocked(*__lock);
int pthread_spin_lock(pthread_spinlock_t *__lock) __locks_exclusive(*__lock);
int pthread_spin_trylock(pthread_spinlock_t *__lock)
    __trylocks_exclusive(0, *__lock);
int pthread_spin_unlock(pthread_spinlock_t *__lock) __unlocks(*__lock);
__END_DECLS

// Optimize pthread_once() by letting the caller check the state.

#if _CLOUDLIBC_INLINE_FUNCTIONS
static __inline int __pthread_once(pthread_once_t *__once_control,
                                   void (*__init_routine)(void)) {
  return __once_control->__state != 0
             ? pthread_once(__once_control, __init_routine)
             : 0;
}
#define pthread_once(once_control, init_routine) \
  __pthread_once(once_control, init_routine)
#endif

#endif
