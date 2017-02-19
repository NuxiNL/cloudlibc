// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include "argdata_impl.h"

argdata_t *argdata_create_timestamp(const struct timespec *ts) {
  // Extract values from the timestamp.
  if (ts->tv_nsec < 0 || ts->tv_nsec >= 1000000000) {
    errno = EINVAL;
    return NULL;
  }
  static_assert(sizeof(time_t) <= sizeof(int64_t), "Timestamp too large");
  int64_t sec = ts->tv_sec;
  uint32_t nsec = ts->tv_nsec;

  // Allocate object with space for encoded timestamp. The timestamp can
  // at most be 12 bytes long.
  static const size_t objlen = sizeof(argdata_t) + 13;
  argdata_t *ad = malloc(objlen);
  if (ad == NULL)
    return NULL;
  uint8_t *buf_end = (uint8_t *)ad + objlen;
  uint8_t *buf = buf_end;

  // Compute the number of nanoseconds since 1970. Start by computing
  // the bottom 32 bits.
  uint64_t low = (uint64_t)(uint32_t)sec * 1000000000 + nsec;
  for (size_t i = 0; i < sizeof(uint32_t); ++i)
    *--buf = low >> i * 8;

  // Computing the top 64 bits, adding the overflow of the bottom 32 bits.
  int64_t high = (sec >> 32) * 1000000000 + (low >> 32);
  for (size_t i = 0; i < sizeof(int64_t); ++i)
    *--buf = high >> i * 8;

  // Trim bytes leading bytes that don't contribute to the value.
  while (buf_end - buf >= 2 && buf[0] == 0 && (buf[1] & 0x80) == 0)
    ++buf;
  while (buf_end - buf >= 2 && buf[0] == 0xff && (buf[1] & 0x80) == 0x80)
    ++buf;
  if (buf_end - buf == 1 && *buf == 0)
    ++buf;
  *--buf = ADT_TIMESTAMP;

  ad->type = AD_BUFFER;
  ad->buffer = buf;
  ad->length = buf_end - buf;
  return ad;
}
