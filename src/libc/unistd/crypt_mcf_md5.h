// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <string.h>

#include "crypt_hash_md5.h"

#define MD5_Update_sequence(ctx, buf, totallen) \
  do {                                          \
    size_t l = (totallen);                      \
    while (l > MD5_DIGEST_LENGTH) {             \
      MD5_Update(ctx, buf, MD5_DIGEST_LENGTH);  \
      l -= MD5_DIGEST_LENGTH;                   \
    }                                           \
    MD5_Update(ctx, buf, l);                    \
  } while (0)

// Creates a salted MD5 hash in the Modular Crypt Format.
static void crypt_mcf_md5(const char *key, const char *salt, char *out) {
  size_t keylen = crypt_keylen(key);
  size_t saltlen = crypt_saltlen(salt, 8);

  // Compute what is similar to SHA's digest B.
  unsigned char B[MD5_DIGEST_LENGTH];
  {
    MD5_CTX c;
    MD5_Init(&c);
    MD5_Update(&c, key, keylen);
    MD5_Update(&c, salt, saltlen);
    MD5_Update(&c, key, keylen);
    MD5_Final(B, &c);
  }

  // Compute what is similar to SHA's digest A.
  unsigned char A_C[MD5_DIGEST_LENGTH];
  {
    MD5_CTX c;
    MD5_Init(&c);
    MD5_Update(&c, key, keylen);
    MD5_Update(&c, "$1$", 3);
    MD5_Update(&c, salt, saltlen);
    MD5_Update_sequence(&c, B, keylen);
    for (size_t i = keylen; i > 0; i >>= 1) {
      if ((i % 2) != 0) {
        char zero = '\0';
        MD5_Update(&c, &zero, 1);
      } else {
        MD5_Update(&c, key, 1);
      }
    }
    MD5_Final(A_C, &c);
  }

  // Compute password hash.
  for (unsigned int r = 0; r < 1000; ++r) {
    MD5_CTX c;
    MD5_Init(&c);
    if ((r % 2) != 0)
      MD5_Update(&c, key, keylen);
    else
      MD5_Update(&c, A_C, sizeof(A_C));
    if ((r % 3) != 0)
      MD5_Update(&c, salt, saltlen);
    if ((r % 7) != 0)
      MD5_Update(&c, key, keylen);
    if ((r % 2) != 0)
      MD5_Update(&c, A_C, sizeof(A_C));
    else
      MD5_Update(&c, key, keylen);
    MD5_Final(A_C, &c);
  }

  // Produce the output string.
  *out++ = '$';
  *out++ = '1';
  *out++ = '$';
  memcpy(out, salt, saltlen);
  out += saltlen;
  *out++ = '$';

  // Base64 encode the hash.
  crypt_base64_3(&out, A_C[12], A_C[6], A_C[0]);
  crypt_base64_3(&out, A_C[13], A_C[7], A_C[1]);
  crypt_base64_3(&out, A_C[14], A_C[8], A_C[2]);
  crypt_base64_3(&out, A_C[15], A_C[9], A_C[3]);
  crypt_base64_3(&out, A_C[5], A_C[10], A_C[4]);
  crypt_base64_1(&out, A_C[11]);
  *out = '\0';
}

#undef MD5_Update_sequence
