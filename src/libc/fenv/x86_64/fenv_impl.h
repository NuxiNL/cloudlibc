// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#ifndef FENV_X86_64_FENV_IMPL_H
#define FENV_X86_64_FENV_IMPL_H

#include <fenv.h>
#include <stdint.h>

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
