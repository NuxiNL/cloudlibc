// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdint.h>
#include <string.h>

// SHA-256 hash computation state.
// http://www.iwar.org.uk/comsec/resources/cipher/sha256-384-512.pdf
typedef struct {
  uint32_t H[8];             // Intermediate hash results.
  uint64_t length;           // Total length in bytes processed.
  unsigned char buffer[64];  // Input buffer for partial blocks.
} SHA256_CTX;

#define SHA256_DIGEST_LENGTH 32

static void SHA256_Init(SHA256_CTX *ctx) {
  // Page 3: Initial hash value.
  ctx->H[0] = 0x6a09e667;
  ctx->H[1] = 0xbb67ae85;
  ctx->H[2] = 0x3c6ef372;
  ctx->H[3] = 0xa54ff53a;
  ctx->H[4] = 0x510e527f;
  ctx->H[5] = 0x9b05688c;
  ctx->H[6] = 0x1f83d9ab;
  ctx->H[7] = 0x5be0cd19;

  ctx->length = 0;
}

// Page 6: Logical functions.
#define S(x, y) ((x) << (32 - (y)) | (x) >> (y))
#define R(x, y) ((x) >> (y))
#define Ch(x, y, z) (((x) & (y)) ^ (~(x) & (z)))
#define Maj(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define Sigma0(x) (S(x, 2) ^ S(x, 13) ^ S(x, 22))
#define Sigma1(x) (S(x, 6) ^ S(x, 11) ^ S(x, 25))
#define sigma0(x) (S(x, 7) ^ S(x, 18) ^ R(x, 3))
#define sigma1(x) (S(x, 17) ^ S(x, 19) ^ R(x, 10))

static void SHA256_Transform(uint32_t *H, const unsigned char *block) {
  // Page 7: Constants table K.
  static const uint32_t K[] = {
      0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1,
      0x923f82a4, 0xab1c5ed5, 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
      0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174, 0xe49b69c1, 0xefbe4786,
      0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
      0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
      0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
      0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85, 0xa2bfe8a1, 0xa81a664b,
      0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
      0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a,
      0x5b9cca4f, 0x682e6ff3, 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
      0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
  };

  // Page 4: Message parsing.
  uint32_t W[16];
  for (size_t j = 0; j < 16; ++j)
    W[j] = (uint32_t)block[j * 4 + 0] << 24 | (uint32_t)block[j * 4 + 1] << 16 |
           (uint32_t)block[j * 4 + 2] << 8 | (uint32_t)block[j * 4 + 3] << 0;

  // Page 5: Main loop.
  uint32_t a = H[0];
  uint32_t b = H[1];
  uint32_t c = H[2];
  uint32_t d = H[3];
  uint32_t e = H[4];
  uint32_t f = H[5];
  uint32_t g = H[6];
  uint32_t h = H[7];
  for (size_t i = 0;; i += 16) {
    // By unrolling eight rounds of the compression function, rotation
    // of the eight registers can be eliminated entirely.
    for (size_t j = 0; j < 16; j += 8) {
      d += h += Sigma1(e) + Ch(e, f, g) + K[i + j] + W[j];
      h += Sigma0(a) + Maj(a, b, c);
      c += g += Sigma1(d) + Ch(d, e, f) + K[i + j + 1] + W[j + 1];
      g += Sigma0(h) + Maj(h, a, b);
      b += f += Sigma1(c) + Ch(c, d, e) + K[i + j + 2] + W[j + 2];
      f += Sigma0(g) + Maj(g, h, a);
      a += e += Sigma1(b) + Ch(b, c, d) + K[i + j + 3] + W[j + 3];
      e += Sigma0(f) + Maj(f, g, h);
      h += d += Sigma1(a) + Ch(a, b, c) + K[i + j + 4] + W[j + 4];
      d += Sigma0(e) + Maj(e, f, g);
      g += c += Sigma1(h) + Ch(h, a, b) + K[i + j + 5] + W[j + 5];
      c += Sigma0(d) + Maj(d, e, f);
      f += b += Sigma1(g) + Ch(g, h, a) + K[i + j + 6] + W[j + 6];
      b += Sigma0(c) + Maj(c, d, e);
      e += a += Sigma1(f) + Ch(f, g, h) + K[i + j + 7] + W[j + 7];
      a += Sigma0(b) + Maj(b, c, d);
    }
    if (i == 48)
      break;

    // Page 6: Expanded message block W.
    for (size_t j = 0; j < 16; ++j)
      W[j] +=
          sigma1(W[(j + 14) % 16]) + W[(j + 9) % 16] + sigma0(W[(j + 1) % 16]);
  }
  H[0] += a;
  H[1] += b;
  H[2] += c;
  H[3] += d;
  H[4] += e;
  H[5] += f;
  H[6] += g;
  H[7] += h;
}

#undef S
#undef R
#undef Ch
#undef Maj
#undef Sigma0
#undef Sigma1
#undef sigma0
#undef sigma1

static void SHA256_Update(SHA256_CTX *ctx, const void *data, size_t len) {
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
    SHA256_Transform(ctx->H, ctx->buffer);

    // Process as many blocks of data without copying them into the
    // input buffer.
    const unsigned char *d = (const unsigned char *)data + remaining;
    size_t l = len - remaining;
    while (l >= sizeof(ctx->buffer)) {
      SHA256_Transform(ctx->H, d);
      d += sizeof(ctx->buffer);
      l -= sizeof(ctx->buffer);
    }

    // Copy remainder into the input buffer.
    memcpy(ctx->buffer, d, l);
  }

  // Update the length counter.
  ctx->length += len;
}

static void SHA256_Final(unsigned char *digest, SHA256_CTX *ctx) {
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
    SHA256_Transform(ctx->H, ctx->buffer);
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
  uint64_t length[1] = {ctx->length << 3};
  encode(&ctx->buffer[sizeof(ctx->buffer) - sizeof(length)], length);
  SHA256_Transform(ctx->H, ctx->buffer);

  // Copy out the final value of the digest.
  encode(digest, ctx->H);
#undef encode
}
