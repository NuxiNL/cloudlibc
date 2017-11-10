// Copyright (c) 2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <stdint.h>
#include <stdlib.h>

long a64l(const char *s) {
  int32_t value = 0;
  for (size_t i = 0; i < 6; ++i) {
    static const char chars[] = {
#define MAP(a, b) [(a) - '.'] = (b) + 1
        MAP('.', 0),  MAP('/', 1),  MAP('0', 2),  MAP('1', 3),  MAP('2', 4),
        MAP('3', 5),  MAP('4', 6),  MAP('5', 7),  MAP('6', 8),  MAP('7', 9),
        MAP('8', 10), MAP('9', 11), MAP('A', 12), MAP('B', 13), MAP('C', 14),
        MAP('D', 15), MAP('E', 16), MAP('F', 17), MAP('G', 18), MAP('H', 19),
        MAP('I', 20), MAP('J', 21), MAP('K', 22), MAP('L', 23), MAP('M', 24),
        MAP('N', 25), MAP('O', 26), MAP('P', 27), MAP('Q', 28), MAP('R', 29),
        MAP('S', 30), MAP('T', 31), MAP('U', 32), MAP('V', 33), MAP('W', 34),
        MAP('X', 35), MAP('Y', 36), MAP('Z', 37), MAP('a', 38), MAP('b', 39),
        MAP('c', 40), MAP('d', 41), MAP('e', 42), MAP('f', 43), MAP('g', 44),
        MAP('h', 45), MAP('i', 46), MAP('j', 47), MAP('k', 48), MAP('l', 49),
        MAP('m', 50), MAP('n', 51), MAP('o', 52), MAP('p', 53), MAP('q', 54),
        MAP('r', 55), MAP('s', 56), MAP('t', 57), MAP('u', 58), MAP('v', 59),
        MAP('w', 60), MAP('x', 61), MAP('y', 62), MAP('z', 63),
#undef MAP
    };
    unsigned char c = s[i] - '.';
    if (c >= __arraycount(chars) || chars[c] == 0)
      break;
    value |= (chars[c] - 1) << i * 6;
  }
  return value;
}
