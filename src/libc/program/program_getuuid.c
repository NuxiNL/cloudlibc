// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <program.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>

static pthread_once_t init = PTHREAD_ONCE_INIT;
static char uuid[37];

static void generate(void) {
  // Generate a fully random UUID as described in RFC-4122.
  uint8_t buf[16];
  arc4random_buf(buf, sizeof(buf));
  buf[6] = (buf[6] & 0x0f) | 0x40;
  buf[8] = (buf[8] & 0x3f) | 0x80;

  // Convert the UUID to a string.
  char *out = uuid;
  for (size_t i = 0; i < sizeof(buf); ++i) {
    if (i == 4 || i == 6 || i == 8 || i == 10)
      *out++ = '-';
    *out++ = "0123456789abcdef"[buf[i] >> 4];
    *out++ = "0123456789abcdef"[buf[i] & 0xf];
  }
  *out = '\0';
}

const char *program_getuuid(void) {
  pthread_once(&init, generate);
  return uuid;
}
