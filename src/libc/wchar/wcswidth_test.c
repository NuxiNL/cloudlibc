// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <testing.h>
#include <wchar.h>

TEST(wcswidth, simple) {
  const wchar_t str[] = L"Iñtërnâtiônàlizætiøn";
  ASSERT_EQ(19, wcswidth(str, __arraycount(str) - 2));
  ASSERT_EQ(20, wcswidth(str, __arraycount(str) - 1));
  ASSERT_EQ(20, wcswidth(str, __arraycount(str)));
  ASSERT_EQ(20, wcswidth(str, __arraycount(str) + 1));
}

TEST(wcswidth, japanese) {
  const wchar_t str[] = L"コンニチハ";
  ASSERT_EQ(8, wcswidth(str, __arraycount(str) - 2));
  ASSERT_EQ(10, wcswidth(str, __arraycount(str) - 1));
  ASSERT_EQ(10, wcswidth(str, __arraycount(str)));
  ASSERT_EQ(10, wcswidth(str, __arraycount(str) + 1));
}

TEST(wcswidth, thai) {
  const wchar_t str[] = L"๏ แผ่นดินฮั่นเสื่อมโทรมแสนสังเวช";
  ASSERT_EQ(24, wcswidth(str, __arraycount(str) - 2));
  ASSERT_EQ(25, wcswidth(str, __arraycount(str) - 1));
  ASSERT_EQ(25, wcswidth(str, __arraycount(str)));
  ASSERT_EQ(25, wcswidth(str, __arraycount(str) + 1));
}

TEST(wcswidth, zalgo) {
  const wchar_t str[] = L"T̫̺̳o̬̜ ì̬͎̲̟nv̖̗̻̣̹̕o͖̗̠̜̤k͍͚̹͖̼e̦̗̪͍̪͍ ̬ͅt̕h̠͙̮͕͓e̱̜̗͙̭ ̥͔̫͙̪͍̣͝ḥi̼̦͈̼v҉̩̟͚̞͎e͈̟̻͙̦̤-m̷̘̝̱í͚̞̦̳n̝̲̯̙̮͞d̴̺̦͕̫ ̗̭̘͎͖r̞͎̜̜͖͎̫͢ep͇r̝̯̝͖͉͎̺e̴s̥e̵̖̳͉͍̩̗n̢͓̪͕̜̰̠̦t̺̞̰i͟n҉̮̦̖̟g̮͍̱̻͍̜̳ ̳c̖̮̙̣̰̠̩h̷̗͍̖͙̭͇͈a̧͎̯̹̲̺̫ó̭̞̜̣̯͕s̶̤̮̩̘.̨̻̪̖͔";
  ASSERT_EQ(43, wcswidth(str, __arraycount(str)));
}
