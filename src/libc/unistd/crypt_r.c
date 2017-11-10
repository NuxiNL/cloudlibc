// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/byteset.h>

#include <errno.h>
#include <stdint.h>
#include <unistd.h>

// Computes the length of a key.
static size_t crypt_keylen(const char *key) {
  // TODO(ed): Should we place an upper limit on this to prevent denial
  // of service attacks?
  return strlen(key);
}

// Base64 character set.
static const char base64[] =
    "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

// Computes the length of a salt string, only allowing characters from
// the Base64 character set.
static size_t crypt_saltlen(const char *salt, size_t max) {
  byteset_t charset;
  byteemptyset(&charset);
  for (const char *c = base64; *c != '\0'; ++c)
    byteaddset(&charset, *c);

  size_t len = 0;
  while (len < max && byteismember(&charset, salt[len]))
    ++len;
  return len;
}

// Base64 encodes one byte to two characters.
static void crypt_base64_1(char **out, unsigned char c1) {
  uint_least8_t v = c1;
  *(*out)++ = base64[v & 0x3f];
  *(*out)++ = base64[v >> 6];
}

// Base64 encodes two bytes to three characters.
static void crypt_base64_2(char **out, unsigned char c1, unsigned char c2) {
  uint_least16_t v = (uint_least16_t)c2 << 8 | c1;
  *(*out)++ = base64[v & 0x3f];
  *(*out)++ = base64[(v >> 6) & 0x3f];
  *(*out)++ = base64[v >> 12];
}

// Base64 encodes three bytes to four characters.
static void crypt_base64_3(char **out, unsigned char c1, unsigned char c2,
                           unsigned char c3) {
  uint_least24_t v = (uint_least24_t)c3 << 16 | (uint_least24_t)c2 << 8 | c1;
  *(*out)++ = base64[v & 0x3f];
  *(*out)++ = base64[(v >> 6) & 0x3f];
  *(*out)++ = base64[(v >> 12) & 0x3f];
  *(*out)++ = base64[v >> 18];
}

#include "crypt_mcf_md5.h"
#include "crypt_mcf_sha256.h"
#include "crypt_mcf_sha512.h"

char *crypt_r(const char *key, const char *salt, struct crypt_data *data) {
  // Pick a hashing algorithm based on the Modular Crypt Format header.
  if (salt[0] == '$' && salt[1] == '1' && salt[2] == '$') {
    crypt_mcf_md5(key, salt + 3, data->__buf);
  } else if (salt[0] == '$' && salt[1] == '5' && salt[2] == '$') {
    crypt_mcf_sha256(key, salt + 3, data->__buf);
  } else if (salt[0] == '$' && salt[1] == '6' && salt[2] == '$') {
    crypt_mcf_sha512(key, salt + 3, data->__buf);
  } else {
    // Unsupported hashing algorithm.
    errno = ENOSYS;
    return NULL;
  }
  return data->__buf;
}
