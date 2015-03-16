// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#ifndef COMMON_CRT_H
#define COMMON_CRT_H

#include <common/syscalldefs.h>

#include <link.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdint.h>
#include <stdnoreturn.h>
#include <threads.h>

// Markers for sections.
extern void (*__ctors_start[])(void);
extern void (*__ctors_stop[])(void);
extern void (*__dtors_start[])(void);
extern void (*__dtors_stop[])(void);

// DSO handle.
// TODO(edje): Why is this needed?
extern void *__dso_handle;

// Stack smashing protection.
extern unsigned long __stack_chk_guard;

// ELF program header.
extern const ElfW(Phdr) * __elf_phdr;
extern ElfW(Half) __elf_phnum;

// Initial thread-local storage data.
extern const void *__tls_init_data;
extern size_t __tls_init_size;
extern size_t __tls_total_size;
extern size_t __tls_alignment;

// Executable entry point.
noreturn void _start(void **, void (*)(void));

// Multi-threading: pthread_t handle and thread ID.
struct __pthread {
  _Atomic(cloudabi_lock_t) join;  // Join queue used by pthread_join().
  void *return_value;             // Value returned by pthread_join().
  void *buffer;                   // Stack buffer used by this thread.

  atomic_uint detachstate;  // Flags related to thread detaching.
#define DETACH_DETACHED 0x1
#define DETACH_TERMINATING 0x2

  void *(*start_routine)(void *);  // User-supplied startup routine.
  void *argument;                  // Argument for startup routine.
};

extern thread_local pthread_t __pthread_self_object;
extern thread_local cloudabi_tid_t __pthread_thread_id;

#endif
