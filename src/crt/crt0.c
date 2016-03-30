// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/argdata.h>
#include <common/crt.h>
#include <common/pthread.h>

#include <cloudabi_syscalls.h>
#include <program.h>
#include <stdalign.h>
#include <stdatomic.h>
#include <threads.h>

// DSO handle. Not used, as we only support static linkage.
void *__dso_handle = NULL;

// Stack protection: stack smashing and LLVM SafeStack.
unsigned long __stack_chk_guard = 0xdeadc0de;
thread_local void *__safestack_unsafe_stack_ptr;
static alignas(PTHREAD_UNSAFE_STACK_ALIGNMENT) char initial_unsafe_stack
    [PTHREAD_STACK_DEFAULT];

// ELF program header.
const ElfW(Phdr) *__elf_phdr = NULL;
ElfW(Half) __elf_phnum = 0;

// Machine properties.
uint32_t __ncpus = 1;
uint32_t __pagesize = 1;

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

noreturn void _start(const cloudabi_auxv_t *auxv) {
  // Extract parameters from the auxiliary vector.
  const void *arg = NULL;
  size_t arglen = 0;
  const void *base = NULL;
  const void *canary = NULL;
  size_t canarylen = 0;
  cloudabi_tid_t tid = 1;
  while (auxv->a_type != CLOUDABI_AT_NULL) {
    switch (auxv->a_type) {
      case CLOUDABI_AT_ARGDATA:
        arg = auxv->a_ptr;
        break;
      case CLOUDABI_AT_ARGDATALEN:
        arglen = auxv->a_val;
        break;
      case CLOUDABI_AT_BASE:
        base = auxv->a_ptr;
        break;
      case CLOUDABI_AT_CANARY:
        canary = auxv->a_ptr;
        break;
      case CLOUDABI_AT_CANARYLEN:
        canarylen = auxv->a_val;
        break;
      case CLOUDABI_AT_NCPUS:
        __ncpus = auxv->a_val;
        break;
      case CLOUDABI_AT_PAGESZ:
        __pagesize = auxv->a_val;
        break;
      case CLOUDABI_AT_PHDR:
        __elf_phdr = auxv->a_ptr;
        break;
      case CLOUDABI_AT_PHNUM:
        __elf_phnum = auxv->a_val;
        break;
      case CLOUDABI_AT_TID:
        tid = auxv->a_val;
        break;
    }
    ++auxv;
  }

  // Set stack smashing guard.
  if (canarylen > sizeof(__stack_chk_guard))
    canarylen = sizeof(__stack_chk_guard);
  crt_memcpy(&__stack_chk_guard, canary, canarylen);

  // Iterate through the program header to obtain values of interest.
  // Also store the location of the program header, so it can be
  // returned in the future by dl_iterate_phdr().
  for (size_t i = 0; i < __elf_phnum; ++i) {
    const ElfW(Phdr) *phdr = &__elf_phdr[i];
    switch (phdr->p_type) {
      case PT_TLS:
        // TLS header. This process uses variables stored in
        // thread-local storage. Extract the location and the size of
        // the initial TLS data.
        __tls_init_data = (const char *)base + phdr->p_vaddr;
        __tls_init_size = phdr->p_filesz;
        __tls_total_size = __roundup(phdr->p_memsz, phdr->p_align);
        __tls_alignment = phdr->p_align;
        break;
    }
  }

  // Set up TLS space for the main thread. Instead of calling malloc()
  // or mmap(), simply allocate a buffer on the stack of this thread.
  char tls_space[__tls_total_size + __tls_alignment - 1];
  char *tls_start = (char *)__roundup((uintptr_t)tls_space, __tls_alignment);
  crt_memcpy(tls_start, __tls_init_data, __tls_init_size);
  crt_memset(tls_start + __tls_init_size, 0,
             __tls_total_size - __tls_init_size);
#if defined(__aarch64__)
  asm volatile("msr tpidr_el0, %0" : : "r"(tls_start - 16));
#elif defined(__x86_64__)
  char *tls_end = tls_start + __tls_total_size;
  cloudabi_sys_thread_tcb_set(&tls_end);
#else
#error "Unsupported architecture"
#endif

  // Set unsafe stack for the initial thread.
  __safestack_unsafe_stack_ptr = &initial_unsafe_stack[PTHREAD_STACK_DEFAULT];

  // Patch up the pthread state for the initial thread. Make sure that
  // the pthread_t for the initial thread is valid. Also adjust
  // __pthread_thread_id to ensure that functions like
  // pthread_mutex_lock() write the proper thread ID into the lock.
  struct __pthread self_object = {
      .join = ATOMIC_VAR_INIT(tid | CLOUDABI_LOCK_WRLOCKED),
  };
  __pthread_self_object = &self_object;
  __pthread_thread_id = tid;

  // Invoke global constructors.
  void (**ctor)(void) = __ctors_stop;
  while (ctor > __ctors_start)
    (*--ctor)();

  // Invoke program_main(). If program_main() is not part of the
  // application, the C library provides a copy that calls main().
  argdata_t ad;
  argdata_init_buffer(&ad, arg, arglen);
  program_main(&ad);
}
