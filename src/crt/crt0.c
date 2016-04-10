// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/argdata.h>
#include <common/crt.h>
#include <common/pthread.h>
#include <common/tls.h>

#include <cloudabi_syscalls.h>
#include <cloudabi_syscalls_names.h>
#include <cloudabi_syscalls_native.h>
#include <cloudabi_syscalls_struct.h>
#include <program.h>
#include <stdalign.h>
#include <stdatomic.h>
#include <threads.h>

// Default system call table.
//
// TODO(ed): Once our work on switching over to a vDSO for providing the
// system call handlers has finished, this table will simply contain a
// list of pointers to a function that returns ENOSYS. For now, fill it
// up with functions that use inline assembly to call into the kernel
// using the "syscall" instruction.
cloudabi_syscalls_t cloudabi_syscalls = {
#define entry(name) .name = cloudabi_native_sys_##name,
    CLOUDABI_SYSCALL_NAMES(entry)
#undef entry
};

// DSO handle. Not used, as we only support static linkage.
void *__dso_handle = NULL;

// Stack protection: stack smashing and LLVM SafeStack.
unsigned long __stack_chk_guard = 0xdeadc0de;
thread_local void *__safestack_unsafe_stack_ptr;
static alignas(PTHREAD_UNSAFE_STACK_ALIGNMENT) char initial_unsafe_stack
    [PTHREAD_STACK_DEFAULT];

// Values preserved from the auxiliary vector.
const void *__at_base;
uint32_t __at_ncpus;
uint32_t __at_pagesz;
const ElfW(Phdr) * __at_phdr;
ElfW(Half) __at_phnum;

// Values preserved from the ELF program headers.
const void *__pt_tls_vaddr_abs;
size_t __pt_tls_filesz;
size_t __pt_tls_memsz_aligned;
size_t __pt_tls_align;

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
  const void *at_argdata = NULL;
  size_t at_argdatalen = 0;
  char *at_base = NULL;
  const void *at_canary = NULL;
  size_t at_canarylen = 0;
  uint32_t at_ncpus = 1;
  uint32_t at_pagesz = 1;
  const ElfW(Phdr) *at_phdr = NULL;
  ElfW(Half) at_phnum = 0;
  cloudabi_tid_t at_tid = 1;
  while (auxv->a_type != CLOUDABI_AT_NULL) {
    switch (auxv->a_type) {
      case CLOUDABI_AT_ARGDATA:
        at_argdata = auxv->a_ptr;
        break;
      case CLOUDABI_AT_ARGDATALEN:
        at_argdatalen = auxv->a_val;
        break;
      case CLOUDABI_AT_BASE:
        at_base = auxv->a_ptr;
        break;
      case CLOUDABI_AT_CANARY:
        at_canary = auxv->a_ptr;
        break;
      case CLOUDABI_AT_CANARYLEN:
        at_canarylen = auxv->a_val;
        break;
      case CLOUDABI_AT_NCPUS:
        at_ncpus = auxv->a_val;
        break;
      case CLOUDABI_AT_PAGESZ:
        at_pagesz = auxv->a_val;
        break;
      case CLOUDABI_AT_PHDR:
        at_phdr = auxv->a_ptr;
        break;
      case CLOUDABI_AT_PHNUM:
        at_phnum = auxv->a_val;
        break;
      case CLOUDABI_AT_TID:
        at_tid = auxv->a_val;
        break;
    }
    ++auxv;
  }

  // Iterate through the program header to obtain values of interest.
  const ElfW(Dyn) *dyn = NULL;
  uintptr_t relro_start;
  size_t relro_size = 0;
  const void *pt_tls_vaddr_abs = 0;
  size_t pt_tls_filesz = 0;
  size_t pt_tls_memsz_aligned = 0;
  size_t pt_tls_align = 1;
  for (size_t i = 0; i < at_phnum; ++i) {
    const ElfW(Phdr) *phdr = &at_phdr[i];
    switch (phdr->p_type) {
      case PT_DYNAMIC:
        // Dynamic Section.
        dyn = (const ElfW(Dyn) *)(at_base + phdr->p_vaddr);
        break;
      case PT_GNU_RELRO:
        // Read-only relocations. Some parts of executable are marked
        // writable in the ELF header, even though they only need to be
        // writable to apply relocations. Determine which exact pages
        // can be made write protected once we've finished relocating.
        relro_start = __roundup((uintptr_t)at_base + phdr->p_vaddr, at_pagesz);
        relro_size =
            __rounddown((uintptr_t)at_base + phdr->p_vaddr + phdr->p_memsz,
                        at_pagesz) -
            relro_start;
        break;
      case PT_TLS:
        // TLS header. This process uses variables stored in
        // thread-local storage. Extract the location and the size of
        // the initial TLS data.
        pt_tls_vaddr_abs = at_base + phdr->p_vaddr;
        pt_tls_filesz = phdr->p_filesz;
        pt_tls_memsz_aligned = __roundup(phdr->p_memsz, phdr->p_align);
        pt_tls_align = phdr->p_align;
        break;
    }
  }

  // Iterate through the Dynamic Section to obtain values of interest.
  const ElfW(Rela) *rela = NULL;
  size_t rela_size = 0;
  if (dyn != NULL) {
    while (dyn->d_tag != DT_NULL) {
      switch (dyn->d_tag) {
        case DT_RELA:
          // Relocations that need to be performed.
          rela = (const ElfW(Rela) *)(at_base + dyn->d_un.d_ptr);
          break;
        case DT_RELASZ:
          // The number of relocations.
          rela_size = dyn->d_un.d_val;
          break;
      }
      ++dyn;
    }
  }

  // Perform relocations. The type of relocations that are performed is machine
  // dependent.
  for (; rela_size >= sizeof(*rela); rela_size -= sizeof(*rela)) {
    char *obj = at_base + rela->r_offset;
    switch (ELFW(R_TYPE)(rela->r_info)) {
#if defined(__aarch64__)
      case R_AARCH64_RELATIVE:
        // Base + Addend.
        *(char **)obj = at_base + rela->r_addend;
        break;
#elif defined(__x86_64__)
      case R_X86_64_RELATIVE:
        // Base + Addend.
        *(char **)obj = at_base + rela->r_addend;
        break;
#else
#error "Unsupported architecture"
#endif
      default:
        // Terminate immediately if there is a relocation that we don't
        // support. Otherwise we end up having hard to debug crashes.
        cloudabi_sys_proc_raise(CLOUDABI_SIGABRT);
        cloudabi_sys_proc_exit(127);
    }
    ++rela;
  }

  // Mark memory that was only made writable to apply relocations read-only.
  if (relro_size > 0)
    cloudabi_sys_mem_protect((void *)relro_start, relro_size,
                             CLOUDABI_PROT_READ);

  // Now that we've relocated, we can write to global memory. Preserve
  // some of the values from the auxiliary vector and program header.
  // Some of these values are returned by sysconf() and
  // dl_iterate_phdr(). Others are needed by pthread_create().
  __at_base = at_base;
  __at_ncpus = at_ncpus;
  __at_pagesz = at_pagesz;
  __at_phdr = at_phdr;
  __at_phnum = at_phnum;
  __pt_tls_vaddr_abs = pt_tls_vaddr_abs;
  __pt_tls_filesz = pt_tls_filesz;
  __pt_tls_memsz_aligned = pt_tls_memsz_aligned;
  __pt_tls_align = pt_tls_align;

  // Set stack smashing guard.
  if (at_canarylen > sizeof(__stack_chk_guard))
    at_canarylen = sizeof(__stack_chk_guard);
  crt_memcpy(&__stack_chk_guard, at_canary, at_canarylen);

  // Set up TLS space for the main thread. Instead of calling malloc()
  // or mmap(), simply allocate a buffer on the stack of this thread.
  char tls_space[tls_size()];
  char *tls_start = tls_addr(tls_space);
  crt_memcpy(tls_start, pt_tls_vaddr_abs, pt_tls_filesz);
  crt_memset(tls_start + pt_tls_filesz, 0,
             pt_tls_memsz_aligned - pt_tls_filesz);
  tls_replace(tls_space);

  // Set unsafe stack for the initial thread.
  __safestack_unsafe_stack_ptr = &initial_unsafe_stack[PTHREAD_STACK_DEFAULT];

  // Patch up the pthread state for the initial thread. Make sure that
  // the pthread_t for the initial thread is valid. Also adjust
  // __pthread_thread_id to ensure that functions like
  // pthread_mutex_lock() write the proper thread ID into the lock.
  struct __pthread self_object = {
      .join = ATOMIC_VAR_INIT(at_tid | CLOUDABI_LOCK_WRLOCKED),
  };
  __pthread_self_object = &self_object;
  __pthread_thread_id = at_tid;

  // Invoke global constructors.
  void (**ctor)(void) = __ctors_stop;
  while (ctor > __ctors_start)
    (*--ctor)();

  // Invoke program_main(). If program_main() is not part of the
  // application, the C library provides a copy that calls main().
  argdata_t ad;
  argdata_init_buffer(&ad, at_argdata, at_argdatalen);
  program_main(&ad);
}
