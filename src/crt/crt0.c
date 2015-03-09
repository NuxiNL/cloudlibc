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

static void handle_program_header(const ElfW(Phdr) * phdr, ElfW(Half) phnum) {
  // Store reference to program header for later use.
  __elf_phdr = phdr;
  __elf_phnum = phnum;

  // Extract locations of commonly used fields (e.g., the location of
  // the initial TLS data).
  for (long i = 0; i < phnum; ++i) {
    switch (phdr->p_type) {
      case PT_TLS:
        __tls_init_data = (const void *)phdr->p_vaddr;
        __tls_init_size = phdr->p_filesz;
        __tls_total_size = __roundup(phdr->p_memsz, phdr->p_align);
        __tls_alignment = phdr->p_align;
        break;
    }
    ++phdr;
  }
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

static void handle_auxiliary_vector(const auxv_t *auxv) {
  // Process fields stored in the auxiliary vector that we support.
  const ElfW(Phdr) *phdr = NULL;
  long phent = 0;
  ElfW(Half) phnum = 0;
  while (auxv->a_type != AT_NULL) {
    switch (auxv->a_type) {
      case AT_PHDR:
        phdr = auxv->a_un.a_ptr;
        break;
      case AT_PHENT:
        phent = auxv->a_un.a_val;
        break;
      case AT_PHNUM:
        phnum = auxv->a_un.a_val;
        break;
    }
    ++auxv;
  }

  // Found the program header. Process it as well.
  if (phdr != NULL && phent == sizeof(*phdr))
    handle_program_header(phdr, phnum);
}

#endif

static void call_ctors(void) {
  // Invoke constructor functions.
  void (**ctor)(void) = __ctors_stop;
  while (ctor > __ctors_start)
    (*--ctor)();
}

static void call_dtors(void) {
  // Invoke destructor functions.
  void (**dtor)(void) = __dtors_start;
  while (dtor < __dtors_stop)
    (*dtor++)();
}

noreturn void _start(void **ap, void (*cleanup)(void)) {
  // TODO(edje): Have this in the auxiliary vector.
  unsigned long stack_chk_guard;
  cloudabi_sys_random_get(&stack_chk_guard, sizeof(stack_chk_guard));
  __stack_chk_guard = stack_chk_guard;

  // Obtain auxiliary vector by scanning past the command line arguments
  // and environment variables.
  // TODO(edje): Remove support for command line arguments and
  // environment variables and replace it by a binary blob.
  int argc = *(long *)ap;
  void **auxv = ap + argc + 2;
  while (*auxv++ != NULL) {
  }
  handle_auxiliary_vector((auxv_t *)auxv);

  // Set up TLS space for the main thread.
  char tls_space[__tls_total_size + __tls_alignment - 1];
  char *tls_start = (char *)__roundup((uintptr_t)tls_space, __tls_alignment);
  char *tls_end = tls_start + __tls_total_size;
  size_t fill = 0;
  while (fill < __tls_init_size) {
    tls_start[fill] = ((const char *)__tls_init_data)[fill];
    ++fill;
  }
  while (fill < __tls_total_size) {
    tls_start[fill] = '\0';
    ++fill;
  }
  cloudabi_sys_thread_tcb_set(&tls_end);

  // Extract the initial thread ID.
  // TODO(edje): Don't do this through poll. Have this data in the
  // auxiliary vector.
  cloudabi_lock_t initial_thread_id;
  {
    _Atomic(cloudabi_lock_t) atomic_tid;
    cloudabi_event_t event = {
        .type = CLOUDABI_EVENT_TYPE_LOCK_WRLOCK, .lock.lock = &atomic_tid,
    };
    size_t triggered;
    cloudabi_sys_poll_once(&event, 1, &event, 1, &triggered);
    initial_thread_id = atomic_load(&atomic_tid);
  }
  initial_thread_id &= ~CLOUDABI_LOCK_WRLOCKED;

  // Fix up some of the variables stored in TLS.
  struct __pthread self_object = {
      .join = ATOMIC_VAR_INIT(initial_thread_id | CLOUDABI_LOCK_WRLOCKED),
  };
  __pthread_self_object = &self_object;
  __pthread_thread_id = initial_thread_id;

  // Invoke global constructors.
  call_ctors();

  // Invoke program_main().
  // TODO(edje): Pass in proper arguments.
  __exit(program_main(NULL, 0));
}

noreturn void __exit(uint8_t ret) {
  // Invoke global destructors.
  call_dtors();

  // Terminate process.
  cloudabi_sys_proc_exit(ret);
}

void *__dso_handle = NULL;

int __cxa_atexit(void (*func)(void *), void *arg, void *dso_handle) {
  // TODO(edje): Implement.
  return 0;
}
