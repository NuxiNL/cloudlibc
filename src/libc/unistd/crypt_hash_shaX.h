// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/overflow.h>

#include <stdint.h>
#include <string.h>

static void SHAX_Update(SHAX_CTX *ctx, const void *data, size_t len) {
  // Insert data into the input buffer.
  size_t used = ctx->length[1] % sizeof(ctx->buffer);
  size_t remaining = sizeof(ctx->buffer) - used;
  if (len < remaining) {
    // Data fits entirely in the input buffer.
    memcpy(&ctx->buffer[used], data, len);
  } else {
    // Data doesn't fit in the input buffer anymore. Finish off the
    // first block.
    memcpy(&ctx->buffer[used], data, remaining);
    SHAX_Transform(ctx->H, ctx->buffer);

    // Process as many blocks of data without copying them into the
    // input buffer.
    const unsigned char *d = (const unsigned char *)data + remaining;
    size_t l = len - remaining;
    while (l >= sizeof(ctx->buffer)) {
      SHAX_Transform(ctx->H, d);
      d += sizeof(ctx->buffer);
      l -= sizeof(ctx->buffer);
    }

    // Copy remainder into the input buffer.
    memcpy(ctx->buffer, d, l);
  }

  // Update the 128-bits length counter.
  uint64_t length[2] = {len >> 32 >> 32, len};
  ctx->length[0] += length[0];
  if (add_overflow(ctx->length[1], length[1], &ctx->length[1]))
    ++ctx->length[0];
}

static void SHAX_Final(unsigned char *digest, SHAX_CTX *ctx) {
  // Append the padding.
  size_t used = ctx->length[1] % sizeof(ctx->buffer);
  static const unsigned char
      padding[sizeof(ctx->buffer) - sizeof(ctx->length)] = {0x80};
  if (used < sizeof(padding)) {
    // There is space for at least one padding byte and the length. Add
    // padding up to the length.
    memcpy(&ctx->buffer[used], padding, sizeof(padding) - used);
  } else {
    // There is not enough space for any padding. Insert another block.
    memcpy(&ctx->buffer[used], padding, sizeof(ctx->buffer) - used);
    SHAX_Transform(ctx->H, ctx->buffer);
    memset(ctx->buffer, '\0', sizeof(padding));
  }

#define encode(ptr, v)                              \
  do {                                              \
    for (size_t i = 0; i < __arraycount(v); ++i)    \
      for (size_t j = 0; j < sizeof((v)[0]); ++j)   \
        (ptr)[i * sizeof((v)[0]) + j] =             \
            (v)[i] >> (sizeof((v)[0]) - 1 - j) * 8; \
  } while (0)
  // Store the length of the entire stream at the end of the last block.
  uint64_t length[2] = {ctx->length[0] << 3 | ctx->length[1] >> 61,
                        ctx->length[1] << 3};
  encode(&ctx->buffer[sizeof(ctx->buffer) - sizeof(length)], length);
  SHAX_Transform(ctx->H, ctx->buffer);

  // Copy out the final value of the digest.
  encode(digest, ctx->H);
#undef encode
}
