// Copyright (c) 2015 Nuxi, https://nuxi.nl/
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

#ifndef COMMON_SYSCALLS_H
#define COMMON_SYSCALLS_H

#include <stdnoreturn.h>

#include "syscalldefs.h"

#if defined(__aarch64__)

typedef uint64_t register_t;

// TODO(ed): Determine whether this is correct.
#define CLOBBERS                                                               \
  "memory", "x0", "x1", "x2", "x3", "x4", "x5", "x6", "x7", "x8", "x9", "x10", \
      "x11", "x12", "x13", "x14", "x15", "x16", "x17", "x18", "d0", "d1",      \
      "d2", "d3", "d4", "d5", "d6", "d7"

#define SYSCALL_0_0(number, name)                                      \
  static inline cloudabi_errno_t cloudabi_sys_##name(void) {           \
    register register_t reg_sc asm("x8") = (number);                   \
    register register_t reg_value1 asm("x0");                          \
    asm volatile("svc 0" : "=r"(reg_value1) : "r"(reg_sc) : CLOBBERS); \
    return reg_value1;                                                 \
  }

#define SYSCALL_0_2(number, name, rtype1, rtype2)                       \
  static inline cloudabi_errno_t cloudabi_sys_##name(rtype1 *result1,   \
                                                     rtype2 *result2) { \
    return CLOUDABI_ENOSYS;                                             \
  }

#define SYSCALL_1_0(number, name, type1)                             \
  static inline cloudabi_errno_t cloudabi_sys_##name(type1 value1) { \
    register register_t reg_sc asm("x8") = (number);                 \
    register register_t reg_value1 asm("x0") = (register_t)(value1); \
    asm volatile("svc 0"                                             \
                 : "=r"(reg_value1)                                  \
                 : "r"(reg_sc), "r"(reg_value1)                      \
                 : CLOBBERS);                                        \
    return reg_value1;                                               \
  }

#define SYSCALL_1_1(number, name, type1, rtype1)                        \
  static inline cloudabi_errno_t cloudabi_sys_##name(type1 value1,      \
                                                     rtype1 *result1) { \
    return CLOUDABI_ENOSYS;                                             \
  }

#define SYSCALL_1_2(number, name, type1, rtype1, rtype2) \
  static inline cloudabi_errno_t cloudabi_sys_##name(    \
      type1 value1, rtype1 *result1, rtype2 *result2) {  \
    return CLOUDABI_ENOSYS;                              \
  }

#define SYSCALL_2_0(number, name, type1, type2)                      \
  static inline cloudabi_errno_t cloudabi_sys_##name(type1 value1,   \
                                                     type2 value2) { \
    register register_t reg_sc asm("x8") = (number);                 \
    register register_t reg_value1 asm("x0") = (register_t)(value1); \
    register register_t reg_value2 asm("x1") = (register_t)(value2); \
    asm volatile("svc 0"                                             \
                 : "=r"(reg_value1)                                  \
                 : "r"(reg_sc), "r"(reg_value1), "r"(reg_value2)     \
                 : CLOBBERS);                                        \
    return reg_value1;                                               \
  }

#define SYSCALL_2_1(number, name, type1, type2, rtype1) \
  static inline cloudabi_errno_t cloudabi_sys_##name(   \
      type1 value1, type2 value2, rtype1 *result1) {    \
    return CLOUDABI_ENOSYS;                             \
  }

#define SYSCALL_3_0(number, name, type1, type2, type3)               \
  static inline cloudabi_errno_t cloudabi_sys_##name(                \
      type1 value1, type2 value2, type3 value3) {                    \
    register register_t reg_sc asm("x8") = (number);                 \
    register register_t reg_value1 asm("x0") = (register_t)(value1); \
    register register_t reg_value2 asm("x1") = (register_t)(value2); \
    register register_t reg_value3 asm("x2") = (register_t)(value3); \
    asm volatile("svc 0"                                             \
                 : "=r"(reg_value1)                                  \
                 : "r"(reg_sc), "r"(reg_value1), "r"(reg_value2),    \
                   "r"(reg_value3)                                   \
                 : CLOBBERS);                                        \
    return reg_value1;                                               \
  }

#define SYSCALL_3_1(number, name, type1, type2, type3, rtype1)           \
  static inline cloudabi_errno_t cloudabi_sys_##name(                    \
      type1 value1, type2 value2, type3 value3, rtype1 *result1) {       \
    register register_t reg_sc asm("x8") = (number);                     \
    register register_t reg_value1 asm("x0") = (register_t)(value1);     \
    register register_t reg_value2 asm("x1") = (register_t)(value2);     \
    register register_t reg_value3 asm("x2") = (register_t)(value3);     \
    register register_t okay;                                            \
    asm volatile(                                                        \
        "\tsvc 0\n"                                                      \
        "\tmov %0, #0\n"                                                 \
        "\tb.cs 1f\n"                                                    \
        "\tmov %0, #1\n"                                                 \
        "1:\n"                                                           \
        : "=r"(okay), "=r"(reg_value1)                                   \
        : "r"(reg_sc), "r"(reg_value1), "r"(reg_value2), "r"(reg_value3) \
        : CLOBBERS);                                                     \
    if (okay) {                                                          \
      *result1 = (rtype1)reg_value1;                                     \
      return 0;                                                          \
    } else {                                                             \
      return reg_value1;                                                 \
    }                                                                    \
  }

#define SYSCALL_4_0(number, name, type1, type2, type3, type4)        \
  static inline cloudabi_errno_t cloudabi_sys_##name(                \
      type1 value1, type2 value2, type3 value3, type4 value4) {      \
    register register_t reg_sc asm("x8") = (number);                 \
    register register_t reg_value1 asm("x0") = (register_t)(value1); \
    register register_t reg_value2 asm("x1") = (register_t)(value2); \
    register register_t reg_value3 asm("x2") = (register_t)(value3); \
    register register_t reg_value4 asm("x3") = (register_t)(value4); \
    asm volatile("svc 0"                                             \
                 : "=r"(reg_value1)                                  \
                 : "r"(reg_sc), "r"(reg_value1), "r"(reg_value2),    \
                   "r"(reg_value3), "r"(reg_value4)                  \
                 : CLOBBERS);                                        \
    return reg_value1;                                               \
  }

#define SYSCALL_4_1(number, name, type1, type2, type3, type4, rtype1) \
  static inline cloudabi_errno_t cloudabi_sys_##name(                 \
      type1 value1, type2 value2, type3 value3, type4 value4,         \
      rtype1 *result1) {                                              \
    return CLOUDABI_ENOSYS;                                           \
  }

#define SYSCALL_5_0(number, name, type1, type2, type3, type4, type5)          \
  static inline cloudabi_errno_t cloudabi_sys_##name(                         \
      type1 value1, type2 value2, type3 value3, type4 value4, type5 value5) { \
    register register_t reg_sc asm("x8") = (number);                          \
    register register_t reg_value1 asm("x0") = (register_t)(value1);          \
    register register_t reg_value2 asm("x1") = (register_t)(value2);          \
    register register_t reg_value3 asm("x2") = (register_t)(value3);          \
    register register_t reg_value4 asm("x3") = (register_t)(value4);          \
    register register_t reg_value5 asm("x4") = (register_t)(value5);          \
    asm volatile("svc 0"                                                      \
                 : "=r"(reg_value1)                                           \
                 : "r"(reg_sc), "r"(reg_value1), "r"(reg_value2),             \
                   "r"(reg_value3), "r"(reg_value4), "r"(reg_value5)          \
                 : CLOBBERS);                                                 \
    return reg_value1;                                                        \
  }

#define SYSCALL_5_1(number, name, type1, type2, type3, type4, type5, rtype1) \
  static inline cloudabi_errno_t cloudabi_sys_##name(                        \
      type1 value1, type2 value2, type3 value3, type4 value4, type5 value5,  \
      rtype1 *result1) {                                                     \
    return CLOUDABI_ENOSYS;                                                  \
  }

#define SYSCALL_6_0(number, name, type1, type2, type3, type4, type5, type6) \
  static inline cloudabi_errno_t cloudabi_sys_##name(                       \
      type1 value1, type2 value2, type3 value3, type4 value4, type5 value5, \
      type6 value6) {                                                       \
    register register_t reg_sc asm("x8") = (number);                        \
    register register_t reg_value1 asm("x0") = (register_t)(value1);        \
    register register_t reg_value2 asm("x1") = (register_t)(value2);        \
    register register_t reg_value3 asm("x2") = (register_t)(value3);        \
    register register_t reg_value4 asm("x3") = (register_t)(value4);        \
    register register_t reg_value5 asm("x4") = (register_t)(value5);        \
    register register_t reg_value6 asm("x5") = (register_t)(value6);        \
    asm volatile("svc 0"                                                    \
                 : "=r"(reg_value1)                                         \
                 : "r"(reg_sc), "r"(reg_value1), "r"(reg_value2),           \
                   "r"(reg_value3), "r"(reg_value4), "r"(reg_value5),       \
                   "r"(reg_value6)                                          \
                 : CLOBBERS);                                               \
    return reg_value1;                                                      \
  }

#define SYSCALL_6_1(number, name, type1, type2, type3, type4, type5, type6, \
                    rtype1)                                                 \
  static inline cloudabi_errno_t cloudabi_sys_##name(                       \
      type1 value1, type2 value2, type3 value3, type4 value4, type5 value5, \
      type6 value6, rtype1 *result1) {                                      \
    return CLOUDABI_ENOSYS;                                                 \
  }

#elif defined(__x86_64__)

typedef uint64_t register_t;

// TODO(ed): Determine whether this is correct.
#define CLOBBERS "memory", "rcx", "rdx", "r8", "r9", "r10", "r11"

#define SYSCALL_0_0(number, name)                                    \
  static inline cloudabi_errno_t cloudabi_sys_##name(void) {         \
    register register_t reg_sc asm("rax") = (number);                \
    asm volatile("syscall" : "=r"(reg_sc) : "r"(reg_sc) : CLOBBERS); \
    return reg_sc;                                                   \
  }

#define SYSCALL_0_2(number, name, rtype1, rtype2)                       \
  static inline cloudabi_errno_t cloudabi_sys_##name(rtype1 *result1,   \
                                                     rtype2 *result2) { \
    register register_t reg_sc asm("rax") = (number);                   \
    register register_t reg_value3 asm("rdx");                          \
    register register_t okay;                                           \
    asm volatile(                                                       \
        "\tsyscall\n"                                                   \
        "\tmov $0, %0\n"                                                \
        "\tjb 1f\n"                                                     \
        "\tmov $1, %0\n"                                                \
        "1:\n"                                                          \
        : "=r"(okay), "=r"(reg_sc), "=r"(reg_value3)                    \
        : "r"(reg_sc)                                                   \
        : CLOBBERS);                                                    \
    if (okay) {                                                         \
      *result1 = (rtype1)reg_sc;                                        \
      *result2 = (rtype2)reg_value3;                                    \
      return 0;                                                         \
    } else {                                                            \
      return reg_sc;                                                    \
    }                                                                   \
  }

#define SYSCALL_1_0(number, name, type1)                              \
  static inline cloudabi_errno_t cloudabi_sys_##name(type1 value1) {  \
    register register_t reg_sc asm("rax") = (number);                 \
    register register_t reg_value1 asm("rdi") = (register_t)(value1); \
    asm volatile("syscall"                                            \
                 : "=r"(reg_sc)                                       \
                 : "r"(reg_sc), "r"(reg_value1)                       \
                 : CLOBBERS);                                         \
    return reg_sc;                                                    \
  }

#define SYSCALL_1_1(number, name, type1, rtype1)                        \
  static inline cloudabi_errno_t cloudabi_sys_##name(type1 value1,      \
                                                     rtype1 *result1) { \
    register register_t reg_sc asm("rax") = (number);                   \
    register register_t reg_value1 asm("rdi") = (register_t)(value1);   \
    register register_t okay;                                           \
    asm volatile(                                                       \
        "\tsyscall\n"                                                   \
        "\tmov $0, %0\n"                                                \
        "\tjb 1f\n"                                                     \
        "\tmov $1, %0\n"                                                \
        "1:\n"                                                          \
        : "=r"(okay), "=r"(reg_sc)                                      \
        : "r"(reg_sc), "r"(reg_value1)                                  \
        : CLOBBERS);                                                    \
    if (okay) {                                                         \
      *result1 = (rtype1)reg_sc;                                        \
      return 0;                                                         \
    } else {                                                            \
      return reg_sc;                                                    \
    }                                                                   \
  }

#define SYSCALL_1_2(number, name, type1, rtype1, rtype2)              \
  static inline cloudabi_errno_t cloudabi_sys_##name(                 \
      type1 value1, rtype1 *result1, rtype2 *result2) {               \
    register register_t reg_sc asm("rax") = (number);                 \
    register register_t reg_value1 asm("rdi") = (register_t)(value1); \
    register register_t reg_value3 asm("rdx");                        \
    register register_t okay;                                         \
    asm volatile(                                                     \
        "\tsyscall\n"                                                 \
        "\tmov $0, %0\n"                                              \
        "\tjb 1f\n"                                                   \
        "\tmov $1, %0\n"                                              \
        "1:\n"                                                        \
        : "=r"(okay), "=r"(reg_sc), "=r"(reg_value3)                  \
        : "r"(reg_sc), "r"(reg_value1)                                \
        : CLOBBERS);                                                  \
    if (okay) {                                                       \
      *result1 = (rtype1)reg_sc;                                      \
      *result2 = (rtype2)reg_value3;                                  \
      return 0;                                                       \
    } else {                                                          \
      return reg_sc;                                                  \
    }                                                                 \
  }

#define SYSCALL_2_0(number, name, type1, type2)                       \
  static inline cloudabi_errno_t cloudabi_sys_##name(type1 value1,    \
                                                     type2 value2) {  \
    register register_t reg_sc asm("rax") = (number);                 \
    register register_t reg_value1 asm("rdi") = (register_t)(value1); \
    register register_t reg_value2 asm("rsi") = (register_t)(value2); \
    asm volatile("syscall"                                            \
                 : "=r"(reg_sc)                                       \
                 : "r"(reg_sc), "r"(reg_value1), "r"(reg_value2)      \
                 : CLOBBERS);                                         \
    return reg_sc;                                                    \
  }

#define SYSCALL_2_1(number, name, type1, type2, rtype1)               \
  static inline cloudabi_errno_t cloudabi_sys_##name(                 \
      type1 value1, type2 value2, rtype1 *result1) {                  \
    register register_t reg_sc asm("rax") = (number);                 \
    register register_t reg_value1 asm("rdi") = (register_t)(value1); \
    register register_t reg_value2 asm("rsi") = (register_t)(value2); \
    register register_t okay;                                         \
    asm volatile(                                                     \
        "\tsyscall\n"                                                 \
        "\tmov $0, %0\n"                                              \
        "\tjb 1f\n"                                                   \
        "\tmov $1, %0\n"                                              \
        "1:\n"                                                        \
        : "=r"(okay), "=r"(reg_sc)                                    \
        : "r"(reg_sc), "r"(reg_value1), "r"(reg_value2)               \
        : CLOBBERS);                                                  \
    if (okay) {                                                       \
      *result1 = (rtype1)reg_sc;                                      \
      return 0;                                                       \
    } else {                                                          \
      return reg_sc;                                                  \
    }                                                                 \
  }

#define SYSCALL_3_0(number, name, type1, type2, type3)                \
  static inline cloudabi_errno_t cloudabi_sys_##name(                 \
      type1 value1, type2 value2, type3 value3) {                     \
    register register_t reg_sc asm("rax") = (number);                 \
    register register_t reg_value1 asm("rdi") = (register_t)(value1); \
    register register_t reg_value2 asm("rsi") = (register_t)(value2); \
    register register_t reg_value3 asm("rdx") = (register_t)(value3); \
    asm volatile("syscall"                                            \
                 : "=r"(reg_sc)                                       \
                 : "r"(reg_sc), "r"(reg_value1), "r"(reg_value2),     \
                   "r"(reg_value3)                                    \
                 : CLOBBERS);                                         \
    return reg_sc;                                                    \
  }

#define SYSCALL_3_1(number, name, type1, type2, type3, rtype1)           \
  static inline cloudabi_errno_t cloudabi_sys_##name(                    \
      type1 value1, type2 value2, type3 value3, rtype1 *result1) {       \
    register register_t reg_sc asm("rax") = (number);                    \
    register register_t reg_value1 asm("rdi") = (register_t)(value1);    \
    register register_t reg_value2 asm("rsi") = (register_t)(value2);    \
    register register_t reg_value3 asm("rdx") = (register_t)(value3);    \
    register register_t okay;                                            \
    asm volatile(                                                        \
        "\tsyscall\n"                                                    \
        "\tmov $0, %0\n"                                                 \
        "\tjb 1f\n"                                                      \
        "\tmov $1, %0\n"                                                 \
        "1:\n"                                                           \
        : "=r"(okay), "=r"(reg_sc)                                       \
        : "r"(reg_sc), "r"(reg_value1), "r"(reg_value2), "r"(reg_value3) \
        : CLOBBERS);                                                     \
    if (okay) {                                                          \
      *result1 = (rtype1)reg_sc;                                         \
      return 0;                                                          \
    } else {                                                             \
      return reg_sc;                                                     \
    }                                                                    \
  }

#define SYSCALL_4_0(number, name, type1, type2, type3, type4)         \
  static inline cloudabi_errno_t cloudabi_sys_##name(                 \
      type1 value1, type2 value2, type3 value3, type4 value4) {       \
    register register_t reg_sc asm("rax") = (number);                 \
    register register_t reg_value1 asm("rdi") = (register_t)(value1); \
    register register_t reg_value2 asm("rsi") = (register_t)(value2); \
    register register_t reg_value3 asm("rdx") = (register_t)(value3); \
    register register_t reg_value4 asm("r10") = (register_t)(value4); \
    asm volatile("syscall"                                            \
                 : "=r"(reg_sc)                                       \
                 : "r"(reg_sc), "r"(reg_value1), "r"(reg_value2),     \
                   "r"(reg_value3), "r"(reg_value4)                   \
                 : CLOBBERS);                                         \
    return reg_sc;                                                    \
  }

#define SYSCALL_4_1(number, name, type1, type2, type3, type4, rtype1)     \
  static inline cloudabi_errno_t cloudabi_sys_##name(                     \
      type1 value1, type2 value2, type3 value3, type4 value4,             \
      rtype1 *result1) {                                                  \
    register register_t reg_sc asm("rax") = (number);                     \
    register register_t reg_value1 asm("rdi") = (register_t)(value1);     \
    register register_t reg_value2 asm("rsi") = (register_t)(value2);     \
    register register_t reg_value3 asm("rdx") = (register_t)(value3);     \
    register register_t reg_value4 asm("r10") = (register_t)(value4);     \
    register register_t okay;                                             \
    asm volatile(                                                         \
        "\tsyscall\n"                                                     \
        "\tmov $0, %0\n"                                                  \
        "\tjb 1f\n"                                                       \
        "\tmov $1, %0\n"                                                  \
        "1:\n"                                                            \
        : "=r"(okay), "=r"(reg_sc)                                        \
        : "r"(reg_sc), "r"(reg_value1), "r"(reg_value2), "r"(reg_value3), \
          "r"(reg_value4)                                                 \
        : CLOBBERS);                                                      \
    if (okay) {                                                           \
      *result1 = (rtype1)reg_sc;                                          \
      return 0;                                                           \
    } else {                                                              \
      return reg_sc;                                                      \
    }                                                                     \
  }

#define SYSCALL_5_0(number, name, type1, type2, type3, type4, type5)          \
  static inline cloudabi_errno_t cloudabi_sys_##name(                         \
      type1 value1, type2 value2, type3 value3, type4 value4, type5 value5) { \
    register register_t reg_sc asm("rax") = (number);                         \
    register register_t reg_value1 asm("rdi") = (register_t)(value1);         \
    register register_t reg_value2 asm("rsi") = (register_t)(value2);         \
    register register_t reg_value3 asm("rdx") = (register_t)(value3);         \
    register register_t reg_value4 asm("r10") = (register_t)(value4);         \
    register register_t reg_value5 asm("r8") = (register_t)(value5);          \
    asm volatile("syscall"                                                    \
                 : "=r"(reg_sc)                                               \
                 : "r"(reg_sc), "r"(reg_value1), "r"(reg_value2),             \
                   "r"(reg_value3), "r"(reg_value4), "r"(reg_value5)          \
                 : CLOBBERS);                                                 \
    return reg_sc;                                                            \
  }

#define SYSCALL_5_1(number, name, type1, type2, type3, type4, type5, rtype1) \
  static inline cloudabi_errno_t cloudabi_sys_##name(                        \
      type1 value1, type2 value2, type3 value3, type4 value4, type5 value5,  \
      rtype1 *result1) {                                                     \
    register register_t reg_sc asm("rax") = (number);                        \
    register register_t reg_value1 asm("rdi") = (register_t)(value1);        \
    register register_t reg_value2 asm("rsi") = (register_t)(value2);        \
    register register_t reg_value3 asm("rdx") = (register_t)(value3);        \
    register register_t reg_value4 asm("r10") = (register_t)(value4);        \
    register register_t reg_value5 asm("r8") = (register_t)(value5);         \
    register register_t okay;                                                \
    asm volatile(                                                            \
        "\tsyscall\n"                                                        \
        "\tmov $0, %0\n"                                                     \
        "\tjb 1f\n"                                                          \
        "\tmov $1, %0\n"                                                     \
        "1:\n"                                                               \
        : "=r"(okay), "=r"(reg_sc)                                           \
        : "r"(reg_sc), "r"(reg_value1), "r"(reg_value2), "r"(reg_value3),    \
          "r"(reg_value4), "r"(reg_value5)                                   \
        : CLOBBERS);                                                         \
    if (okay) {                                                              \
      *result1 = (rtype1)reg_sc;                                             \
      return 0;                                                              \
    } else {                                                                 \
      return reg_sc;                                                         \
    }                                                                        \
  }

#define SYSCALL_6_0(number, name, type1, type2, type3, type4, type5, type6) \
  static inline cloudabi_errno_t cloudabi_sys_##name(                       \
      type1 value1, type2 value2, type3 value3, type4 value4, type5 value5, \
      type6 value6) {                                                       \
    register register_t reg_sc asm("rax") = (number);                       \
    register register_t reg_value1 asm("rdi") = (register_t)(value1);       \
    register register_t reg_value2 asm("rsi") = (register_t)(value2);       \
    register register_t reg_value3 asm("rdx") = (register_t)(value3);       \
    register register_t reg_value4 asm("r10") = (register_t)(value4);       \
    register register_t reg_value5 asm("r8") = (register_t)(value5);        \
    register register_t reg_value6 asm("r9") = (register_t)(value6);        \
    asm volatile("syscall"                                                  \
                 : "=r"(reg_sc)                                             \
                 : "r"(reg_sc), "r"(reg_value1), "r"(reg_value2),           \
                   "r"(reg_value3), "r"(reg_value4), "r"(reg_value5),       \
                   "r"(reg_value6)                                          \
                 : CLOBBERS);                                               \
    return reg_sc;                                                          \
  }

#define SYSCALL_6_1(number, name, type1, type2, type3, type4, type5, type6, \
                    rtype1)                                                 \
  static inline cloudabi_errno_t cloudabi_sys_##name(                       \
      type1 value1, type2 value2, type3 value3, type4 value4, type5 value5, \
      type6 value6, rtype1 *result1) {                                      \
    register register_t reg_sc asm("rax") = (number);                       \
    register register_t reg_value1 asm("rdi") = (register_t)(value1);       \
    register register_t reg_value2 asm("rsi") = (register_t)(value2);       \
    register register_t reg_value3 asm("rdx") = (register_t)(value3);       \
    register register_t reg_value4 asm("r10") = (register_t)(value4);       \
    register register_t reg_value5 asm("r8") = (register_t)(value5);        \
    register register_t reg_value6 asm("r9") = (register_t)(value6);        \
    register register_t okay;                                               \
    asm volatile(                                                           \
        "\tsyscall\n"                                                       \
        "\tmov $0, %0\n"                                                    \
        "\tjb 1f\n"                                                         \
        "\tmov $1, %0\n"                                                    \
        "1:\n"                                                              \
        : "=r"(okay), "=r"(reg_sc)                                          \
        : "r"(reg_sc), "r"(reg_value1), "r"(reg_value2), "r"(reg_value3),   \
          "r"(reg_value4), "r"(reg_value5), "r"(reg_value6)                 \
        : CLOBBERS);                                                        \
    if (okay) {                                                             \
      *result1 = (rtype1)reg_sc;                                            \
      return 0;                                                             \
    } else {                                                                \
      return reg_sc;                                                        \
    }                                                                       \
  }

#else
#error "Unsupported architecture"
#endif

#define SYSCALL_0_N(number, name)                          \
  SYSCALL_0_0(number, _##name);                            \
  static inline _Noreturn void cloudabi_sys_##name(void) { \
    cloudabi_sys__##name();                                \
    for (;;)                                               \
      ;                                                    \
  }

#define SYSCALL_1_N(number, name, type1)                           \
  SYSCALL_1_0(number, _##name, type1);                             \
  static inline _Noreturn void cloudabi_sys_##name(type1 value1) { \
    cloudabi_sys__##name(value1);                                  \
    for (;;)                                                       \
      ;                                                            \
  }

#define SYSCALL_2_N(number, name, type1, type2)                    \
  SYSCALL_2_0(number, _##name, type1, type2);                      \
  static inline _Noreturn void cloudabi_sys_##name(type1 value1,   \
                                                   type2 value2) { \
    cloudabi_sys__##name(value1, value2);                          \
    for (;;)                                                       \
      ;                                                            \
  }

#include "syscalllist.h"

#undef SYSCALL_0_0
#undef SYSCALL_0_2
#undef SYSCALL_0_N
#undef SYSCALL_1_0
#undef SYSCALL_1_1
#undef SYSCALL_1_2
#undef SYSCALL_1_N
#undef SYSCALL_2_0
#undef SYSCALL_2_1
#undef SYSCALL_2_N
#undef SYSCALL_3_0
#undef SYSCALL_3_1
#undef SYSCALL_4_0
#undef SYSCALL_4_1
#undef SYSCALL_5_0
#undef SYSCALL_5_1
#undef SYSCALL_6_0
#undef SYSCALL_6_1

#endif
