// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define SHAX_Update_sequence(ctx, buf, totallen) \
  do {                                           \
    size_t l = (totallen);                       \
    while (l > SHAX_DIGEST_LENGTH) {             \
      SHAX_Update(ctx, buf, SHAX_DIGEST_LENGTH); \
      l -= SHAX_DIGEST_LENGTH;                   \
    }                                            \
    SHAX_Update(ctx, buf, l);                    \
  } while (0)

// Creates a salted SHA hash in the Modular Crypt Format.
// https://www.akkadia.org/drepper/SHA-crypt.txt
static void crypt_mcf_shaX(const char *key, const char *salt, char *out) {
  static const unsigned int rounds_min = 1000;
  static const unsigned int rounds_default = 5000;
  static const unsigned int rounds_max = 999999999;
  static const char rounds_prefix[7] = "rounds=";

  // Parse the "rounds=" attribute.
  unsigned int rounds = rounds_default;
  bool rounds_specified = false;
  if (memcmp(salt, rounds_prefix, sizeof(rounds_prefix)) == 0) {
    char *end;
    unsigned long r = strtoul(salt + sizeof(rounds_prefix), &end, 10);
    if (*end == '$') {
      // Valid number specified.
      rounds_specified = true;
      rounds = r < rounds_min ? rounds_min : r > rounds_max ? rounds_max : r;
      salt = end + 1;
    }
  }

  // Compute the length of the key and the salt.
  size_t keylen = crypt_keylen(key);
  size_t saltlen = crypt_saltlen(salt, 16);

  // Steps 4-8: compute digest B.
  unsigned char B[SHAX_DIGEST_LENGTH];
  {
    SHAX_CTX c;
    SHAX_Init(&c);
    SHAX_Update(&c, key, keylen);
    SHAX_Update(&c, salt, saltlen);
    SHAX_Update(&c, key, keylen);
    SHAX_Final(B, &c);
  }

  // Steps 1-3 and 9-12: compute digest A.
  unsigned char A_C[SHAX_DIGEST_LENGTH];
  {
    SHAX_CTX c;
    SHAX_Init(&c);
    SHAX_Update(&c, key, keylen);
    SHAX_Update(&c, salt, saltlen);
    SHAX_Update_sequence(&c, B, keylen);
    for (size_t i = keylen; i > 0; i >>= 1) {
      if ((i % 2) != 0)
        SHAX_Update(&c, B, sizeof(B));
      else
        SHAX_Update(&c, key, keylen);
    }
    SHAX_Final(A_C, &c);
  }

  // Steps 13-15: compute digest DP.
  unsigned char DP[SHAX_DIGEST_LENGTH];
  {
    SHAX_CTX c;
    SHAX_Init(&c);
    for (size_t i = 0; i < keylen; ++i)
      SHAX_Update(&c, key, keylen);
    SHAX_Final(DP, &c);
  }

  // Steps 17-19: compute digest DS.
  unsigned char DS[SHAX_DIGEST_LENGTH];
  {
    SHAX_CTX c;
    SHAX_Init(&c);
    for (int i = 0; i < 16 + A_C[0]; ++i)
      SHAX_Update(&c, salt, saltlen);
    SHAX_Final(DS, &c);
  }

  // Step 21: compute password hash.
  for (unsigned int r = 0; r < rounds; ++r) {
    SHAX_CTX c;
    SHAX_Init(&c);
    if ((r % 2) != 0)
      SHAX_Update_sequence(&c, DP, keylen);
    else
      SHAX_Update(&c, A_C, sizeof(A_C));
    if ((r % 3) != 0)
      SHAX_Update_sequence(&c, DS, saltlen);
    if ((r % 7) != 0)
      SHAX_Update_sequence(&c, DP, keylen);
    if ((r % 2) != 0)
      SHAX_Update(&c, A_C, sizeof(A_C));
    else
      SHAX_Update_sequence(&c, DP, keylen);
    SHAX_Final(A_C, &c);
  }

  // Step 22: produce the output string.
  *out++ = '$';
  *out++ = SHAX_DIGEST_LENGTH == 32 ? '5' : '6';
  *out++ = '$';
  if (rounds_specified) {
    // Add the number of rounds to the output if specified as input.
    memcpy(out, rounds_prefix, sizeof(rounds_prefix));
    out += sizeof(rounds_prefix);
    char str[sizeof(unsigned int) * 4];
    char *s = &str[sizeof(str)];
    do {
      *--s = rounds % 10 + '0';
      rounds /= 10;
    } while (rounds > 0);
    size_t slen = &str[sizeof(str)] - s;
    memcpy(out, s, slen);
    out += slen;
    *out++ = '$';
  }
  memcpy(out, salt, saltlen);
  out += saltlen;
  *out++ = '$';

  // Base64 encode the hash.
  static const unsigned char map1[] = BASE64_REMAP1;
  static const unsigned char map2[] = BASE64_REMAP2;
  static const unsigned char map3[] = BASE64_REMAP3;
  for (size_t i = 0; i < sizeof(A_C) / 3; ++i)
    crypt_base64_3(&out, A_C[map1[i]], A_C[map2[i]], A_C[map3[i]]);
  switch (sizeof(A_C) % 3) {
    case 1: {
      size_t i = sizeof(A_C) / 3;
      crypt_base64_1(&out, A_C[map1[i]]);
      break;
    }
    case 2: {
      size_t i = sizeof(A_C) / 3;
      crypt_base64_2(&out, A_C[map1[i]], A_C[map2[i]]);
      break;
    }
  }
  *out = '\0';
}

#undef SHAX_Update_sequence
