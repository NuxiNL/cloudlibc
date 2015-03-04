// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// This file is distrbuted under a 2-clause BSD license.
// See the LICENSE file for details.

#include <stddef.h>
#include <stdlib.h>

// qsort() implementation from Bentley and McIlroy's
// "Engineering a Sort Function".

typedef long WORD;

#define min(a, b) ((a) < (b) ? (a) : (b))

#define exch(a, b, t) \
  do {                \
    t = a;            \
    a = b;            \
    b = t;            \
  } while (0)
#define swap(a, b)                         \
  do {                                     \
    if (swaptype != 0)                     \
      swapfunc(a, b, es, swaptype);        \
    else                                   \
      exch(*(WORD *)(a), *(WORD *)(b), t); \
  } while (0)
#define vecswap(a, b, n)           \
  do {                             \
    if (n > 0)                     \
      swapfunc(a, b, n, swaptype); \
  } while (0)

#define SWAPINIT(a, es)                                                        \
  do {                                                                         \
    swaptype =                                                                 \
        ((a - (char *)0) | es) % sizeof(WORD) ? 2 : es > sizeof(WORD) ? 1 : 0; \
  } while (0)
#define PVINIT(pv, pm)   \
  do {                   \
    if (swaptype != 0) { \
      pv = a;            \
      swap(pv, pm);      \
    } else {             \
      pv = (char *)&v;   \
      v = *(WORD *)pm;   \
    }                    \
  } while (0)

static void swapfunc(char *a, char *b, size_t n, int swaptype) {
  if (swaptype <= 1) {
    WORD t;
    for (; n > 0; a += sizeof(WORD), b += sizeof(WORD), n -= sizeof(WORD))
      exch(*(WORD *)a, *(WORD *)b, t);
  } else {
    char t;
    for (; n > 0; a += 1, b += 1, n -= 1)
      exch(*a, *b, t);
  }
}

static char *med3(char *a, char *b, char *c,
                  int (*cmp)(const void *, const void *)) {
  return cmp(a, b) < 0 ? (cmp(b, c) < 0 ? b : cmp(a, c) < 0 ? c : a)
                       : (cmp(b, c) > 0 ? b : cmp(a, c) > 0 ? c : a);
}

void qsort(void *base, size_t n, size_t es,
           int (*cmp)(const void *, const void *)) {
  char *a = base;
  char *pa, *pb, *pc, *pd, *pl, *pm, *pn, *pv;
  int r, swaptype;
  WORD t, v;
  size_t s;
  SWAPINIT(a, es);
  // Insertion sort on smallest arrays.
  if (n < 7) {
    for (pm = a + es; pm < a + n * es; pm += es)
      for (pl = pm; pl > a && cmp(pl - es, pl) > 0; pl -= es)
        swap(pl, pl - es);
    return;
  }
  // Small arrays, middle element.
  pm = a + (n / 2) * es;
  if (n > 7) {
    pl = a;
    pn = a + (n - 1) * es;
    // Big arrays, pseudomedian of 9.
    if (n > 40) {
      s = (n / 8) * es;
      pl = med3(pl, pl + s, pl + 2 * s, cmp);
      pm = med3(pm - s, pm, pm + s, cmp);
      pn = med3(pn - 2 * s, pn - s, pn, cmp);
    }
    // Mid-size, med of 3.
    pm = med3(pl, pm, pn, cmp);
  }
  // pv points to partition value.
  PVINIT(pv, pm);
  pa = pb = a;
  pc = pd = a + (n - 1) * es;
  for (;;) {
    while (pb <= pc && (r = cmp(pb, pv)) <= 0) {
      if (r == 0) {
        swap(pa, pb);
        pa += es;
      }
      pb += es;
    }
    while (pc >= pb && (r = cmp(pc, pv)) >= 0) {
      if (r == 0) {
        swap(pc, pd);
        pd -= es;
      }
      pc -= es;
    }
    if (pb > pc)
      break;
    swap(pb, pc);
    pb += es;
    pc -= es;
  }
  pn = a + n * es;
  s = min(pa - a, pb - pa);
  vecswap(a, pb - s, s);
  s = min(pd - pc, pn - pd - (ptrdiff_t)es);
  vecswap(pb, pn - s, s);
  if ((s = pb - pa) > es)
    qsort(a, s / es, es, cmp);
  if ((s = pd - pc) > es)
    qsort(pn - s, s / es, es, cmp);
}
