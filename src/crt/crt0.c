// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/crt.h>
#include <common/syscalls.h>

#include <program.h>

// DSO handle.
void *__dso_handle = NULL;

// Stack smashing protection.
unsigned long __stack_chk_guard;

// ELF program header.
const ElfW(Phdr) * __elf_phdr;
ElfW(Half) __elf_phnum;

// Machine properties.
uint32_t __ncpus;
uint32_t __pagesize;

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

noreturn void _start(const cloudabi_startup_data_t *sdp, size_t sdplen) {
  cloudabi_startup_data_t sd;
  if (sdplen < sizeof(sd)) {
    // Kernel provided a smaller startup data structure. Add zero padding.
    crt_memcpy(&sd, sdp, sdplen);
    crt_memset((char *)&sd + sdplen, '\0', sizeof(sd) - sdplen);
  } else {
    // Kernel provided a larger startup data structure. Truncate.
    crt_memcpy(&sd, sdp, sizeof(sd));
  }

  // Set stack smashing guard.
  __stack_chk_guard = sd.sd_random_seed;

  // Extract machine properties.
  __ncpus = sd.sd_ncpus;
  __pagesize = sd.sd_pagesize;

  // Iterate through the program header to obtain values of interest.
  // Also store the location of the program header, so it can be
  // returned in the future by dl_iterate_phdr().
  __elf_phdr = sd.sd_elf_phdr;
  __elf_phnum = sd.sd_elf_phdrlen;
  for (size_t i = 0; i < __elf_phnum; ++i) {
    const ElfW(Phdr) *phdr = &__elf_phdr[i];
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
      .join = ATOMIC_VAR_INIT(sd.sd_thread_id | CLOUDABI_LOCK_WRLOCKED),
  };
  __pthread_self_object = &self_object;
  __pthread_thread_id = sd.sd_thread_id;

  // Invoke global constructors.
  void (**ctor)(void) = __ctors_stop;
  while (ctor > __ctors_start)
    (*--ctor)();

  // Invoke program_main(). If program_main() is not part of the
  // application, the C library provides a copy that calls main().
  program_main(sd.sd_arg, sd.sd_arglen);
}
