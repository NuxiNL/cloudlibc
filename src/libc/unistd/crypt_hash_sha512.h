// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/overflow.h>

#include <stdint.h>
#include <string.h>

// SHA-512 hash computation state.
// http://www.iwar.org.uk/comsec/resources/cipher/sha256-384-512.pdf
typedef struct {
  uint64_t H[8];              // Intermediate hash results.
  uint64_t length[2];         // Total length in bytes processed.
  unsigned char buffer[128];  // Input buffer for partial blocks.
} SHA512_CTX;

#define SHA512_DIGEST_LENGTH 64

static void SHA512_Init(SHA512_CTX *ctx) {
  // Page 17: Initial hash value.
  ctx->H[0] = 0x6a09e667f3bcc908;
  ctx->H[1] = 0xbb67ae8584caa73b;
  ctx->H[2] = 0x3c6ef372fe94f82b;
  ctx->H[3] = 0xa54ff53a5f1d36f1;
  ctx->H[4] = 0x510e527fade682d1;
  ctx->H[5] = 0x9b05688c2b3e6c1f;
  ctx->H[6] = 0x1f83d9abfb41bd6b;
  ctx->H[7] = 0x5be0cd19137e2179;

  ctx->length[0] = 0;
  ctx->length[1] = 0;
}

// Page 20: Logical functions.
#define S(x, y) ((x) << (64 - (y)) | (x) >> (y))
#define R(x, y) ((x) >> (y))
#define Ch(x, y, z) (((x) & (y)) ^ (~(x) & (z)))
#define Maj(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define Sigma0(x) (S(x, 28) ^ S(x, 34) ^ S(x, 39))
#define Sigma1(x) (S(x, 14) ^ S(x, 18) ^ S(x, 41))
#define sigma0(x) (S(x, 1) ^ S(x, 8) ^ R(x, 7))
#define sigma1(x) (S(x, 19) ^ S(x, 61) ^ R(x, 6))

static void SHA512_Transform(uint64_t *H, const unsigned char *block) {
  // Page 21: Constants table K.
  static const uint64_t K[] = {
      0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f,
      0xe9b5dba58189dbbc, 0x3956c25bf348b538, 0x59f111f1b605d019,
      0x923f82a4af194f9b, 0xab1c5ed5da6d8118, 0xd807aa98a3030242,
      0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
      0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235,
      0xc19bf174cf692694, 0xe49b69c19ef14ad2, 0xefbe4786384f25e3,
      0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65, 0x2de92c6f592b0275,
      0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
      0x983e5152ee66dfab, 0xa831c66d2db43210, 0xb00327c898fb213f,
      0xbf597fc7beef0ee4, 0xc6e00bf33da88fc2, 0xd5a79147930aa725,
      0x06ca6351e003826f, 0x142929670a0e6e70, 0x27b70a8546d22ffc,
      0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
      0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6,
      0x92722c851482353b, 0xa2bfe8a14cf10364, 0xa81a664bbc423001,
      0xc24b8b70d0f89791, 0xc76c51a30654be30, 0xd192e819d6ef5218,
      0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8,
      0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99,
      0x34b0bcb5e19b48a8, 0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb,
      0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3, 0x748f82ee5defb2fc,
      0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec,
      0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915,
      0xc67178f2e372532b, 0xca273eceea26619c, 0xd186b8c721c0c207,
      0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178, 0x06f067aa72176fba,
      0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b,
      0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc,
      0x431d67c49c100d4c, 0x4cc5d4becb3e42b6, 0x597f299cfc657e2a,
      0x5fcb6fab3ad6faec, 0x6c44198c4a475817,
  };

  // Page 18: Message parsing.
  uint64_t W[16];
  for (size_t j = 0; j < 16; ++j)
    W[j] = (uint64_t)block[j * 8 + 0] << 56 | (uint64_t)block[j * 8 + 1] << 48 |
           (uint64_t)block[j * 8 + 2] << 40 | (uint64_t)block[j * 8 + 3] << 32 |
           (uint64_t)block[j * 8 + 4] << 24 | (uint64_t)block[j * 8 + 5] << 16 |
           (uint64_t)block[j * 8 + 6] << 8 | (uint64_t)block[j * 8 + 7] << 0;

  // Page 19: Main loop.
  uint64_t a = H[0];
  uint64_t b = H[1];
  uint64_t c = H[2];
  uint64_t d = H[3];
  uint64_t e = H[4];
  uint64_t f = H[5];
  uint64_t g = H[6];
  uint64_t h = H[7];
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
    if (i == 64)
      break;

    // Page 20: Expanded message block W.
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

static void SHA512_Update(SHA512_CTX *ctx, const void *data, size_t len) {
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
    SHA512_Transform(ctx->H, ctx->buffer);

    // Process as many blocks of data without copying them into the
    // input buffer.
    const unsigned char *d = (const unsigned char *)data + remaining;
    size_t l = len - remaining;
    while (l >= sizeof(ctx->buffer)) {
      SHA512_Transform(ctx->H, d);
      d += sizeof(ctx->buffer);
      l -= sizeof(ctx->buffer);
    }

    // Copy remainder into the input buffer.
    memcpy(ctx->buffer, d, l);
  }

  // Update the length counter.
  uint64_t length[2] = {(uintmax_t)len >> 32 >> 32, len};
  ctx->length[0] += length[0];
  if (add_overflow(ctx->length[1], length[1], &ctx->length[1]))
    ++ctx->length[0];
}

static void SHA512_Final(unsigned char *digest, SHA512_CTX *ctx) {
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
    SHA512_Transform(ctx->H, ctx->buffer);
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
  SHA512_Transform(ctx->H, ctx->buffer);

  // Copy out the final value of the digest.
  encode(digest, ctx->H);
#undef encode
}
