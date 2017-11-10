// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdint.h>
#include <string.h>

// MD5 hash computation state.
// https://tools.ietf.org/html/rfc1321
typedef struct {
  uint32_t H[4];             // Intermediate hash results.
  uint64_t length;           // Total length in bytes processed.
  unsigned char buffer[64];  // Input buffer for partial blocks.
} MD5_CTX;

#define MD5_DIGEST_LENGTH 16

static void MD5_Init(MD5_CTX *ctx) {
  // Initial hash value.
  ctx->H[0] = 0x67452301;
  ctx->H[1] = 0xefcdab89;
  ctx->H[2] = 0x98badcfe;
  ctx->H[3] = 0x10325476;

  ctx->length = 0;
}

// Logical functions.
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n))))
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))
#define FF(a, b, c, d, x, s, ac)          \
  {                                       \
    (a) += F((b), (c), (d)) + (x) + (ac); \
    (a) = ROTATE_LEFT((a), (s));          \
    (a) += (b);                           \
  }
#define GG(a, b, c, d, x, s, ac)          \
  {                                       \
    (a) += G((b), (c), (d)) + (x) + (ac); \
    (a) = ROTATE_LEFT((a), (s));          \
    (a) += (b);                           \
  }
#define HH(a, b, c, d, x, s, ac)          \
  {                                       \
    (a) += H((b), (c), (d)) + (x) + (ac); \
    (a) = ROTATE_LEFT((a), (s));          \
    (a) += (b);                           \
  }
#define II(a, b, c, d, x, s, ac)          \
  {                                       \
    (a) += I((b), (c), (d)) + (x) + (ac); \
    (a) = ROTATE_LEFT((a), (s));          \
    (a) += (b);                           \
  }

static void MD5_Transform(uint32_t *H, const unsigned char *block) {
  // Decode message.
  uint32_t W[16];
  for (size_t j = 0; j < 16; ++j)
    W[j] = (uint32_t)block[j * 4 + 0] << 0 | (uint32_t)block[j * 4 + 1] << 8 |
           (uint32_t)block[j * 4 + 2] << 16 | (uint32_t)block[j * 4 + 3] << 24;

  uint32_t a = H[0];
  uint32_t b = H[1];
  uint32_t c = H[2];
  uint32_t d = H[3];

  // Round 1.
  FF(a, b, c, d, W[0], 7, 0xd76aa478);
  FF(d, a, b, c, W[1], 12, 0xe8c7b756);
  FF(c, d, a, b, W[2], 17, 0x242070db);
  FF(b, c, d, a, W[3], 22, 0xc1bdceee);
  FF(a, b, c, d, W[4], 7, 0xf57c0faf);
  FF(d, a, b, c, W[5], 12, 0x4787c62a);
  FF(c, d, a, b, W[6], 17, 0xa8304613);
  FF(b, c, d, a, W[7], 22, 0xfd469501);
  FF(a, b, c, d, W[8], 7, 0x698098d8);
  FF(d, a, b, c, W[9], 12, 0x8b44f7af);
  FF(c, d, a, b, W[10], 17, 0xffff5bb1);
  FF(b, c, d, a, W[11], 22, 0x895cd7be);
  FF(a, b, c, d, W[12], 7, 0x6b901122);
  FF(d, a, b, c, W[13], 12, 0xfd987193);
  FF(c, d, a, b, W[14], 17, 0xa679438e);
  FF(b, c, d, a, W[15], 22, 0x49b40821);

  // Round 2.
  GG(a, b, c, d, W[1], 5, 0xf61e2562);
  GG(d, a, b, c, W[6], 9, 0xc040b340);
  GG(c, d, a, b, W[11], 14, 0x265e5a51);
  GG(b, c, d, a, W[0], 20, 0xe9b6c7aa);
  GG(a, b, c, d, W[5], 5, 0xd62f105d);
  GG(d, a, b, c, W[10], 9, 0x2441453);
  GG(c, d, a, b, W[15], 14, 0xd8a1e681);
  GG(b, c, d, a, W[4], 20, 0xe7d3fbc8);
  GG(a, b, c, d, W[9], 5, 0x21e1cde6);
  GG(d, a, b, c, W[14], 9, 0xc33707d6);
  GG(c, d, a, b, W[3], 14, 0xf4d50d87);
  GG(b, c, d, a, W[8], 20, 0x455a14ed);
  GG(a, b, c, d, W[13], 5, 0xa9e3e905);
  GG(d, a, b, c, W[2], 9, 0xfcefa3f8);
  GG(c, d, a, b, W[7], 14, 0x676f02d9);
  GG(b, c, d, a, W[12], 20, 0x8d2a4c8a);

  // Round 3.
  HH(a, b, c, d, W[5], 4, 0xfffa3942);
  HH(d, a, b, c, W[8], 11, 0x8771f681);
  HH(c, d, a, b, W[11], 16, 0x6d9d6122);
  HH(b, c, d, a, W[14], 23, 0xfde5380c);
  HH(a, b, c, d, W[1], 4, 0xa4beea44);
  HH(d, a, b, c, W[4], 11, 0x4bdecfa9);
  HH(c, d, a, b, W[7], 16, 0xf6bb4b60);
  HH(b, c, d, a, W[10], 23, 0xbebfbc70);
  HH(a, b, c, d, W[13], 4, 0x289b7ec6);
  HH(d, a, b, c, W[0], 11, 0xeaa127fa);
  HH(c, d, a, b, W[3], 16, 0xd4ef3085);
  HH(b, c, d, a, W[6], 23, 0x4881d05);
  HH(a, b, c, d, W[9], 4, 0xd9d4d039);
  HH(d, a, b, c, W[12], 11, 0xe6db99e5);
  HH(c, d, a, b, W[15], 16, 0x1fa27cf8);
  HH(b, c, d, a, W[2], 23, 0xc4ac5665);

  // Round 4.
  II(a, b, c, d, W[0], 6, 0xf4292244);
  II(d, a, b, c, W[7], 10, 0x432aff97);
  II(c, d, a, b, W[14], 15, 0xab9423a7);
  II(b, c, d, a, W[5], 21, 0xfc93a039);
  II(a, b, c, d, W[12], 6, 0x655b59c3);
  II(d, a, b, c, W[3], 10, 0x8f0ccc92);
  II(c, d, a, b, W[10], 15, 0xffeff47d);
  II(b, c, d, a, W[1], 21, 0x85845dd1);
  II(a, b, c, d, W[8], 6, 0x6fa87e4f);
  II(d, a, b, c, W[15], 10, 0xfe2ce6e0);
  II(c, d, a, b, W[6], 15, 0xa3014314);
  II(b, c, d, a, W[13], 21, 0x4e0811a1);
  II(a, b, c, d, W[4], 6, 0xf7537e82);
  II(d, a, b, c, W[11], 10, 0xbd3af235);
  II(c, d, a, b, W[2], 15, 0x2ad7d2bb);
  II(b, c, d, a, W[9], 21, 0xeb86d391);

  H[0] += a;
  H[1] += b;
  H[2] += c;
  H[3] += d;
}

#undef ROTATE_LEFT
#undef F
#undef G
#undef H
#undef I
#undef FF
#undef GG
#undef HH
#undef II

static void MD5_Update(MD5_CTX *ctx, const void *data, size_t len) {
  // Insert data into the input buffer.
  size_t used = ctx->length % sizeof(ctx->buffer);
  size_t remaining = sizeof(ctx->buffer) - used;
  if (len < remaining) {
    // Data fits entirely in the input buffer.
    memcpy(&ctx->buffer[used], data, len);
  } else {
    // Data doesn't fit in the input buffer anymore. Finish off the
    // first block.
    memcpy(&ctx->buffer[used], data, remaining);
    MD5_Transform(ctx->H, ctx->buffer);

    // Process as many blocks of data without copying them into the
    // input buffer.
    const unsigned char *d = (const unsigned char *)data + remaining;
    size_t l = len - remaining;
    while (l >= sizeof(ctx->buffer)) {
      MD5_Transform(ctx->H, d);
      d += sizeof(ctx->buffer);
      l -= sizeof(ctx->buffer);
    }

    // Copy remainder into the input buffer.
    memcpy(ctx->buffer, d, l);
  }

  // Update the length counter.
  ctx->length += len;
}

static void MD5_Final(unsigned char *digest, MD5_CTX *ctx) {
  // Append the padding.
  size_t used = ctx->length % sizeof(ctx->buffer);
  static const unsigned char
      padding[sizeof(ctx->buffer) - sizeof(ctx->length)] = {0x80};
  if (used < sizeof(padding)) {
    // There is space for at least one padding byte and the length. Add
    // padding up to the length.
    memcpy(&ctx->buffer[used], padding, sizeof(padding) - used);
  } else {
    // There is not enough space for any padding. Insert another block.
    memcpy(&ctx->buffer[used], padding, sizeof(ctx->buffer) - used);
    MD5_Transform(ctx->H, ctx->buffer);
    memset(ctx->buffer, '\0', sizeof(padding));
  }

#define encode(ptr, v)                                   \
  do {                                                   \
    for (size_t i = 0; i < __arraycount(v); ++i)         \
      for (size_t j = 0; j < sizeof((v)[0]); ++j)        \
        (ptr)[i * sizeof((v)[0]) + j] = (v)[i] >> j * 8; \
  } while (0)
  // Store the length of the entire stream at the end of the last block.
  uint64_t length[1] = {ctx->length << 3};
  encode(&ctx->buffer[sizeof(ctx->buffer) - sizeof(length)], length);
  MD5_Transform(ctx->H, ctx->buffer);

  // Copy out the final value of the digest.
  encode(digest, ctx->H);
#undef encode
}
