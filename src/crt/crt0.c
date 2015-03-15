// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/crt.h>
#include <common/syscalls.h>

#include <program.h>

// Stack smashing protection.
unsigned long __stack_chk_guard = 0xdeadc0de;

// ELF program header.
const ElfW(Phdr) *__elf_phdr = NULL;
ElfW(Half) __elf_phnum = 0;

// Initial thread-local storage data.
const void *__tls_init_data = NULL;
size_t __tls_init_size = 0;
size_t __tls_total_size = 0;
size_t __tls_alignment = 1;

// Multi-threading: pthread_t handle and thread ID.
thread_local pthread_t __pthread_self_object;
thread_local cloudabi_tid_t __pthread_thread_id;

// Simple string functions so we don't depend on libc.
static void crt_memcpy(void *s1, const void *s2, size_t n) {
  char *sb1 = s1;
  const char *sb2 = s2;
  while (n-- > 0)
    *sb1++ = *sb2++;
}

static void crt_memset(void *s, int c, size_t n) {
  char *sb = s;
  while (n-- > 0)
    *sb++ = c;
}

static noreturn void mi_startup(const void *arg, size_t arglen,
                                const ElfW(Phdr) * phdr, ElfW(Half) phnum,
                                cloudabi_tid_t pthread_thread_id,
                                unsigned long stack_chk_guard) {
  // Set stack smashing guard.
  __stack_chk_guard = stack_chk_guard;

  // Iterate through the program header to obtain values of interest.
  // Also store the location of the program header, so it can be
  // returned in the future by dl_iterate_phdr().
  __elf_phdr = phdr;
  __elf_phnum = phnum;
  for (long i = 0; i < phnum; ++i) {
    switch (phdr->p_type) {
      case PT_TLS:
        // TLS header. This process uses variables stored in
        // thread-local storage. Extract the location and the size of
        // the initial TLS data.
        __tls_init_data = (const void *)phdr->p_vaddr;
        __tls_init_size = phdr->p_filesz;
        __tls_total_size = __roundup(phdr->p_memsz, phdr->p_align);
        __tls_alignment = phdr->p_align;
        break;
    }
    ++phdr;
  }

  // Set up TLS space for the main thread. Instead of calling malloc()
  // or mmap(), simply allocate a buffer on the stack of this thread.
  char tls_space[__tls_total_size + __tls_alignment - 1];
  char *tls_start = (char *)__roundup((uintptr_t)tls_space, __tls_alignment);
  char *tls_end = tls_start + __tls_total_size;
  crt_memcpy(tls_start, __tls_init_data, __tls_init_size);
  crt_memset(tls_start + __tls_init_size, 0,
             __tls_total_size - __tls_init_size);
  cloudabi_sys_thread_tcb_set(&tls_end);

  // Patch up the pthread state for the initial thread. Make sure that
  // the pthread_t for the initial thread is valid. Also adjust
  // __pthread_thread_id to ensure that functions like
  // pthread_mutex_lock() write the proper thread ID into the lock.
  struct __pthread self_object = {
      .join = ATOMIC_VAR_INIT(pthread_thread_id | CLOUDABI_LOCK_WRLOCKED),
  };
  __pthread_self_object = &self_object;
  __pthread_thread_id = pthread_thread_id;

  // Invoke global constructors.
  void (**ctor)(void) = __ctors_stop;
  while (ctor > __ctors_start)
    (*--ctor)();

  // Invoke program_main(). If program_main() is not part of the
  // application, the C library provides a copy that calls main().
  __exit(program_main(arg, arglen));
}

noreturn void __exit(uint8_t ret) {
  // Invoke global destructors.
  void (**dtor)(void) = __dtors_start;
  while (dtor < __dtors_stop)
    (*dtor++)();

  // Terminate process.
  cloudabi_sys_proc_exit(ret);
}

void *__dso_handle = NULL;

int __cxa_atexit(void (*func)(void *), void *arg, void *dso_handle) {
  // TODO(edje): Implement.
  return 0;
}

#ifdef __x86_64__

// Auxiliary vector layout and identifiers as described in the x86-64 ABI.
//
// http://www.x86-64.org/documentation/abi.pdf

typedef struct {
  int a_type;
  union {
    long a_val;
    void *a_ptr;
    void (*a_fnc)();
  } a_un;
} auxv_t;

#define AT_NULL 0
#define AT_PHDR 3
#define AT_PHENT 4
#define AT_PHNUM 5

noreturn void _start(void **ap, void (*cleanup)(void)) {
  // TODO(edje): Patch up the kernel to provide these values in a saner
  // way. We also want to support binary blob arguments.

  // Obtain auxiliary vector by scanning past the command line arguments
  // and environment variables.
  int argc = *(long *)ap;
  void **auxvptr = ap + argc + 2;
  while (*auxvptr++ != NULL) {
  }
  auxv_t *auxv = (auxv_t *)auxvptr;

  // Process fields stored in the auxiliary vector that we support.
  const ElfW(Phdr) *phdr = NULL;
  ElfW(Half) phnum = 0;
  while (auxv->a_type != AT_NULL) {
    switch (auxv->a_type) {
      case AT_PHDR:
        phdr = auxv->a_un.a_ptr;
        break;
      case AT_PHNUM:
        phnum = auxv->a_un.a_val;
        break;
    }
    ++auxv;
  }

  // Extract the initial thread ID.
  cloudabi_lock_t pthread_thread_id;
  {
    _Atomic(cloudabi_lock_t) atomic_tid;
    cloudabi_event_t event = {
        .type = CLOUDABI_EVENT_TYPE_LOCK_WRLOCK, .lock.lock = &atomic_tid,
    };
    size_t triggered;
    cloudabi_sys_poll_once(&event, 1, &event, 1, &triggered);
    pthread_thread_id = atomic_load(&atomic_tid);
  }
  pthread_thread_id &= ~CLOUDABI_LOCK_WRLOCKED;

  // Generate stack smashing guard.
  unsigned long stack_chk_guard;
  cloudabi_sys_random_get(&stack_chk_guard, sizeof(stack_chk_guard));

  // Call into the machine-independent startup sequence.
  mi_startup(NULL, 0, phdr, phnum, pthread_thread_id, stack_chk_guard);
}

#else
#error "Unsupported architecture"
#endif
