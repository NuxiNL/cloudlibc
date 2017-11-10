// Copyright (c) 2016-2017 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/crt.h>

#include <program.h>
#include <pthread.h>
#include <stdint.h>

static char uuid[sizeof("e2a38354-c462-11e7-ae9b-d43d7ef8eef2")];

static void generate(void) {
  // Don't crash if the environment did not provide a PID. This will
  // leave the UUID empty, which is obviously incorrect.
  if (__at_pid == NULL)
    return;

  // Convert the UUID provided by the environment to a string.
  char *out = uuid;
  for (size_t i = 0; i < 16; ++i) {
    if (i == 4 || i == 6 || i == 8 || i == 10)
      *out++ = '-';
    *out++ = "0123456789abcdef"[__at_pid[i] >> 4];
    *out++ = "0123456789abcdef"[__at_pid[i] & 0xf];
  }
  *out = '\0';
}

const char *program_getpid(void) {
  static pthread_once_t init = PTHREAD_ONCE_INIT;
  pthread_once(&init, generate);
  return uuid;
}
