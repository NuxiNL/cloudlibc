// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#ifndef FENV_FENV_IMPL_H
#define FENV_FENV_IMPL_H

#include <fenv.h>
#include <stdint.h>

#if defined(__aarch64__)

// Bits 22 and 23 that determine the rounding.
#define ROUNDING_MASK 0xc00000

// Assembly for floating point register manipulation.

static inline uint32_t mrs_fpcr(void) {
  uint64_t cr;
  asm volatile("mrs %0, fpcr" : "=r"(cr));
  return cr;
}

static inline uint32_t mrs_fpsr(void) {
  uint64_t sr;
  asm volatile("mrs %0, fpsr" : "=r"(sr));
  return sr;
}

static inline void msr_fpcr(uint32_t value) {
  uint64_t cr = value;
  asm volatile("msr fpcr, %0" : : "r"(cr));
}

static inline void msr_fpsr(uint32_t value) {
  uint64_t sr = value;
  asm volatile("msr fpsr, %0" : : "r"(sr));
}

#elif defined(__arm__)

// Bits 22 and 23 that determine the rounding.
#define ROUNDING_MASK 0xc00000

// Assembly for floating point register manipulation.

static inline uint32_t vmrs_fpscr(void) {
  uint32_t scr;
  asm volatile("vmrs %0, fpscr" : "=r"(scr));
  return scr;
}

static inline void vmsr_fpscr(uint32_t value) {
  asm volatile("vmsr fpscr, %0" : : "r"(value));
}

#elif defined(__i386__) || defined(__x86_64__)

// Bits 13 and 14 that determine the rounding.
#define ROUNDING_MASK 0x6000

// Exception flags in x87 are the same as for SSE, but shifted by seven
// bits.
#define EXCEPTION_MASK_SHIFT 7

// Assembly for x87 floating point register manipulation.

static inline void fldcw(uint16_t cw) {
  asm volatile("fldcw %0" : : "m"(cw));
}

static inline void fldenv(const struct __x87_state *state) {
  asm volatile("fldenv %0"
               :
               : "m"(*state)
               : "st", "st(1)", "st(2)", "st(3)", "st(4)", "st(5)", "st(6)",
                 "st(7)");
}

static inline uint16_t fnstcw(void) {
  uint16_t cw;
  asm volatile("fnstcw %0" : "=m"(cw));
  return cw;
}

static inline uint16_t fnstsw(void) {
  uint16_t sw;
  asm volatile("fnstsw %0" : "=m"(sw));
  return sw;
}

static inline void fnstenv(struct __x87_state *state) {
  asm volatile("fnstenv %0" : "=m"(*state));
}

static inline void fnclex(void) {
  asm volatile("fnclex");
}

static inline void fwait(void) {
  asm volatile("fwait");
}

// Assembly for SSE floating point register manipulation.

static inline void ldmxcsr(uint32_t mxcsr) {
  asm volatile("ldmxcsr %0" : : "m"(mxcsr));
}

static inline uint32_t stmxcsr(void) {
  uint32_t mxcsr;
  asm volatile("stmxcsr %0" : "=m"(mxcsr));
  return mxcsr;
}

#endif

#endif
