// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <common/spritz.h>

#include "gtest/gtest.h"

TEST(spritz, test_vectors) {
  // There is no way to reliably test arc4random_buf(). At least test
  // that the underlying Spritz drip function returns values that
  // correspond to the test vectors.
  //
  // These test vectors have been obtained from Appendix E of the Spritz
  // paper.
  {
    struct spritz s = SPRITZ_INITIALIZER;
    spritz_absorb(&s, (const uint8_t *)"ABC", 3);
    ASSERT_EQ(0x77, spritz_drip(&s));
    ASSERT_EQ(0x9a, spritz_drip(&s));
    ASSERT_EQ(0x8e, spritz_drip(&s));
    ASSERT_EQ(0x01, spritz_drip(&s));
    ASSERT_EQ(0xf9, spritz_drip(&s));
    ASSERT_EQ(0xe9, spritz_drip(&s));
    ASSERT_EQ(0xcb, spritz_drip(&s));
    ASSERT_EQ(0xc0, spritz_drip(&s));
  }
  {
    struct spritz s = SPRITZ_INITIALIZER;
    spritz_absorb(&s, (const uint8_t *)"spam", 4);
    ASSERT_EQ(0xf0, spritz_drip(&s));
    ASSERT_EQ(0x60, spritz_drip(&s));
    ASSERT_EQ(0x9a, spritz_drip(&s));
    ASSERT_EQ(0x1d, spritz_drip(&s));
    ASSERT_EQ(0xf1, spritz_drip(&s));
    ASSERT_EQ(0x43, spritz_drip(&s));
    ASSERT_EQ(0xce, spritz_drip(&s));
    ASSERT_EQ(0xbf, spritz_drip(&s));
  }
  {
    struct spritz s = SPRITZ_INITIALIZER;
    spritz_absorb(&s, (const uint8_t *)"arcfour", 7);
    ASSERT_EQ(0x1a, spritz_drip(&s));
    ASSERT_EQ(0xfa, spritz_drip(&s));
    ASSERT_EQ(0x8b, spritz_drip(&s));
    ASSERT_EQ(0x5e, spritz_drip(&s));
    ASSERT_EQ(0xe3, spritz_drip(&s));
    ASSERT_EQ(0x37, spritz_drip(&s));
    ASSERT_EQ(0xdb, spritz_drip(&s));
    ASSERT_EQ(0xc7, spritz_drip(&s));
  }
}
