// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <common/syscalls.h>

#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>

// "Spritz -- a spongy RC4-like stream cipher and hash function"
// by Ronald L. Rivest and Jacob C.N. Schuldt.
// http://people.csail.mit.edu/rivest/pubs/RS14.pdf
// TODO(ed): Replace this by something faster (e.g., Keccak).

#define N 256
#define Sw(n) S[(n) % N]

struct spritz {
  uint8_t S[N];
  uint8_t a;
  uint8_t i;
  uint8_t j;
  uint8_t k;
  uint8_t w;
  uint8_t z;
};

static unsigned int spritz_gcd(unsigned int a, unsigned int b) {
  while (b != 0) {
    unsigned int t = b;
    b = a % b;
    a = t;
  }
  return a;
}

static void spritz_swap(uint8_t *a, uint8_t *b) {
  uint8_t tmp = *a;
  *a = *b;
  *b = tmp;
}

static void spritz_initialize_state(struct spritz *s) {
  s->i = s->j = s->k = s->z = s->a = 0;
  s->w = 1;
  for (int v = 0; v < N; ++v)
    s->S[v] = v;
}

static void spritz_update(struct spritz *s) {
  s->i += s->w;
  s->j = s->k + s->Sw(s->j + s->S[s->i]);
  s->k = s->i + s->k + s->S[s->j];
  spritz_swap(&s->S[s->i], &s->S[s->j]);
}

static void spritz_whip(struct spritz *s, unsigned int r) {
  for (unsigned int v = 0; v < r; ++v)
    spritz_update(s);
  do {
    ++s->w;
  } while (spritz_gcd(s->w, N) != 1);
}

static void spritz_crush(struct spritz *s) {
  for (uint8_t v = 0; v < N / 2; ++v)
    if (s->S[v] > s->S[N - 1 - v])
      spritz_swap(&s->S[v], &s->S[N - 1 - v]);
}

static void spritz_shuffle(struct spritz *s) {
  spritz_whip(s, 2 * N);
  spritz_crush(s);
  spritz_whip(s, 2 * N);
  spritz_crush(s);
  spritz_whip(s, 2 * N);
  s->a = 0;
}

static void spritz_absorb_nibble(struct spritz *s, uint8_t x) {
  if (s->a == N / 2)
    spritz_shuffle(s);
  spritz_swap(&s->S[s->a], &s->S[N / 2 + x]);
  ++s->a;
}

static void spritz_absorb_byte(struct spritz *s, uint8_t b) {
  spritz_absorb_nibble(s, b & 0xf);
  spritz_absorb_nibble(s, b >> 4);
}

static void spritz_absorb(struct spritz *s, const uint8_t *i, size_t ilen) {
  while (ilen-- > 0)
    spritz_absorb_byte(s, *i++);
}

static uint8_t spritz_output(struct spritz *s) {
  s->z = s->Sw(s->j + s->Sw(s->i + s->Sw(s->z + s->k)));
  return s->z;
}

static uint8_t spritz_drip(struct spritz *s) {
  if (s->a > 0)
    spritz_shuffle(s);
  spritz_update(s);
  return spritz_output(s);
}

#undef N
#undef Sw

static pthread_mutex_t arc4random_lock;
static struct spritz arc4random_state __guarded_by(arc4random_lock);
static size_t arc4random_bytesleft __guarded_by(arc4random_lock);

// Reinitializes the randomizer state. This function is called on
// startup, but also right after forking. This ensures that the parent
// and child processes will use a different randomizer state.
static void arc4random_reinitialize(void) __no_lock_analysis {
  pthread_mutex_init(&arc4random_lock, NULL);
  spritz_initialize_state(&arc4random_state);
  arc4random_bytesleft = 0;
}

// Installs a fork handler and initializes the randomizer state.
static void arc4random_initialize(void) {
  pthread_atfork(NULL, NULL, arc4random_reinitialize);
  arc4random_reinitialize();
}

// Stirs the Spritz state with a random key.
static void arc4random_stir_locked(void) __requires_exclusive(arc4random_lock) {
  uint8_t key[128];
  if (cloudabi_sys_random_get(&key, sizeof(key)) != 0)
    abort();
  spritz_absorb(&arc4random_state, key, sizeof(key));

  // Force another stir after returning one megabyte of data.
  arc4random_bytesleft = 1 << 20;
}

// Returns a buffer of random data.
void arc4random_buf(void *buf, size_t nbytes) {
  // Ensure that the randomizer has been initialized.
  static pthread_once_t arc4random_once = PTHREAD_ONCE_INIT;
  pthread_once(&arc4random_once, arc4random_initialize);

  uint8_t *b = buf;
  pthread_mutex_lock(&arc4random_lock);
  while (nbytes-- > 0) {
    if (arc4random_bytesleft-- == 0)
      arc4random_stir_locked();
    *b++ = spritz_drip(&arc4random_state);
  }
  pthread_mutex_unlock(&arc4random_lock);
}
