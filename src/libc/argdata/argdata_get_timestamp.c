// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/argdata.h>
#include <common/overflow.h>

#include <argdata.h>
#include <assert.h>
#include <errno.h>
#include <time.h>

int argdata_get_timestamp(const argdata_t *ad, struct timespec *value) {
  switch (ad->type) {
    case AD_BUFFER: {
      const uint8_t *buf = ad->buffer;
      size_t len = ad->length;
      int error = parse_type(ADT_TIMESTAMP, &buf, &len);
      if (error != 0)
        return error;

      // Extract up to 12 bytes from the object. That should be enough
      // to store a 64-bits time_t and the nanoseconds. Pad leading part
      // for signedness.
      uint8_t payload[12];
      if (len > sizeof(payload))
        return ERANGE;
      for (size_t i = 0; i < len; ++i)
        payload[i + sizeof(payload) - len] = buf[i];
      uint8_t pad = len > 0 && (*buf & 0x80) != 0 ? 0xff : 0x00;
      for (size_t i = 0; i < sizeof(payload) - len; ++i)
        payload[i] = pad;

      // Extract the top 64 bits and the bottom 32 bits.
      int64_t high = 0;
      for (size_t i = 0; i < sizeof(high); ++i)
        high = high << 8 | payload[i];
      uint64_t low = 0;
      for (size_t i = sizeof(high); i < sizeof(payload); ++i)
        low = low << 8 | payload[i];

      // Perform division to extract nanoseconds on top 64 bits.
      int32_t high_rem = high % 1000000000;
      high /= 1000000000;
      if (high_rem < 0) {
        high_rem += 1000000000;
        --high;
      }
      if (high < INT32_MIN || high > INT32_MAX)
        return ERANGE;

      // Add remainder to the lower 32 bits and continue division.
      low += (uint64_t)high_rem << 32;
      uint32_t nsec = low % 1000000000;
      low /= 1000000000;

      // Store result in struct timespec.
      static_assert(sizeof(time_t) == sizeof(int64_t),
                    "Timestamp not the right size");
      if (add_overflow(high << 32, low, &value->tv_sec))
        return ERANGE;
      value->tv_nsec = nsec;
      return 0;
    }
    default:
      return EINVAL;
  }
}
