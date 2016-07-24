// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// This file is distributed under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stdint.h>

// SHA-256 hash computation state.
// http://www.iwar.org.uk/comsec/resources/cipher/sha256-384-512.pdf
typedef struct {
  uint32_t H[8];             // Intermediate hash results.
  uint64_t length[2];        // Total length in bytes processed.
  unsigned char buffer[64];  // Input buffer for partial blocks.
} SHA256_CTX;

#define SHA256_DIGEST_LENGTH 32

static void SHA256_Init(SHA256_CTX *ctx) {
  // Page 3: Initial hash value.
  ctx->H[0] = UINT32_C(0x6a09e667);
  ctx->H[1] = UINT32_C(0xbb67ae85);
  ctx->H[2] = UINT32_C(0x3c6ef372);
  ctx->H[3] = UINT32_C(0xa54ff53a);
  ctx->H[4] = UINT32_C(0x510e527f);
  ctx->H[5] = UINT32_C(0x9b05688c);
  ctx->H[6] = UINT32_C(0x1f83d9ab);
  ctx->H[7] = UINT32_C(0x5be0cd19);

  ctx->length[0] = 0;
  ctx->length[1] = 0;
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
      UINT32_C(0x428a2f98), UINT32_C(0x71374491), UINT32_C(0xb5c0fbcf),
      UINT32_C(0xe9b5dba5), UINT32_C(0x3956c25b), UINT32_C(0x59f111f1),
      UINT32_C(0x923f82a4), UINT32_C(0xab1c5ed5), UINT32_C(0xd807aa98),
      UINT32_C(0x12835b01), UINT32_C(0x243185be), UINT32_C(0x550c7dc3),
      UINT32_C(0x72be5d74), UINT32_C(0x80deb1fe), UINT32_C(0x9bdc06a7),
      UINT32_C(0xc19bf174), UINT32_C(0xe49b69c1), UINT32_C(0xefbe4786),
      UINT32_C(0x0fc19dc6), UINT32_C(0x240ca1cc), UINT32_C(0x2de92c6f),
      UINT32_C(0x4a7484aa), UINT32_C(0x5cb0a9dc), UINT32_C(0x76f988da),
      UINT32_C(0x983e5152), UINT32_C(0xa831c66d), UINT32_C(0xb00327c8),
      UINT32_C(0xbf597fc7), UINT32_C(0xc6e00bf3), UINT32_C(0xd5a79147),
      UINT32_C(0x06ca6351), UINT32_C(0x14292967), UINT32_C(0x27b70a85),
      UINT32_C(0x2e1b2138), UINT32_C(0x4d2c6dfc), UINT32_C(0x53380d13),
      UINT32_C(0x650a7354), UINT32_C(0x766a0abb), UINT32_C(0x81c2c92e),
      UINT32_C(0x92722c85), UINT32_C(0xa2bfe8a1), UINT32_C(0xa81a664b),
      UINT32_C(0xc24b8b70), UINT32_C(0xc76c51a3), UINT32_C(0xd192e819),
      UINT32_C(0xd6990624), UINT32_C(0xf40e3585), UINT32_C(0x106aa070),
      UINT32_C(0x19a4c116), UINT32_C(0x1e376c08), UINT32_C(0x2748774c),
      UINT32_C(0x34b0bcb5), UINT32_C(0x391c0cb3), UINT32_C(0x4ed8aa4a),
      UINT32_C(0x5b9cca4f), UINT32_C(0x682e6ff3), UINT32_C(0x748f82ee),
      UINT32_C(0x78a5636f), UINT32_C(0x84c87814), UINT32_C(0x8cc70208),
      UINT32_C(0x90befffa), UINT32_C(0xa4506ceb), UINT32_C(0xbef9a3f7),
      UINT32_C(0xc67178f2),
  };

  // Page 6: Expanded message block W.
  uint32_t W[64];
  for (size_t j = 0; j < 16; ++j)
    W[j] = (uint32_t)block[j * 4 + 0] << 24 | (uint32_t)block[j * 4 + 1] << 16 |
           (uint32_t)block[j * 4 + 2] << 8 | (uint32_t)block[j * 4 + 3] << 0;
  for (size_t j = 16; j < 64; ++j)
    W[j] = sigma1(W[j - 2]) + W[j - 7] + sigma0(W[j - 15]) + W[j - 16];

  // Page 5: Main loop.
  uint32_t a = H[0];
  uint32_t b = H[1];
  uint32_t c = H[2];
  uint32_t d = H[3];
  uint32_t e = H[4];
  uint32_t f = H[5];
  uint32_t g = H[6];
  uint32_t h = H[7];
  for (int j = 0; j < 64; ++j) {
    uint32_t T1 = h + Sigma1(e) + Ch(e, f, g) + K[j] + W[j];
    uint32_t T2 = Sigma0(a) + Maj(a, b, c);
    h = g;
    g = f;
    f = e;
    e = d + T1;
    d = c;
    c = b;
    b = a;
    a = T1 + T2;
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

#define SHAX_CTX SHA256_CTX
#define SHAX_Final SHA256_Final
#define SHAX_Transform SHA256_Transform
#define SHAX_Update SHA256_Update
#include "crypt_hash_shaX.h"
#undef SHAX_CTX
#undef SHAX_Final
#undef SHAX_Transform
#undef SHAX_Update
