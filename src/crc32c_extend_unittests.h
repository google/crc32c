// Copyright 2017 The CRC32C Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef CRC32C_CRC32C_EXTEND_UNITTESTS_H_
#define CRC32C_CRC32C_EXTEND_UNITTESTS_H_

#include <cstddef>
#include <cstdint>
#include <cstring>

#include "gtest/gtest.h"

// Common test cases for all implementations of CRC32C_Extend().
// TESTED_EXTEND should be #defined to the tested function's name.

// TESTED_EXTEND_CASE_NAME ends up with a test case name that depends on
// TESTED_EXTEND.
// The technique is described in http://stackoverflow.com/a/1489985
#define TESTED_EXTEND_PASTER(x, y) x##y
#define TESTED_EXTEND_CASE_NAMER(x, y) TESTED_EXTEND_PASTER(x, y)
#define TESTED_EXTEND_CASE_NAME TESTED_EXTEND_CASE_NAMER(TESTED_EXTEND, Test)

namespace crc32c {

TEST(TESTED_EXTEND_CASE_NAME, StandardResults) {
  // From rfc3720 section B.4.
  uint8_t buf[32];

  std::memset(buf, 0, sizeof(buf));
  EXPECT_EQ(static_cast<uint32_t>(0x8a9136aa),
            TESTED_EXTEND(0, buf, sizeof(buf)));

  std::memset(buf, 0xff, sizeof(buf));
  EXPECT_EQ(static_cast<uint32_t>(0x62a8ab43),
            TESTED_EXTEND(0, buf, sizeof(buf)));

  for (int i = 0; i < 32; ++i)
    buf[i] = static_cast<uint8_t>(i);
  EXPECT_EQ(static_cast<uint32_t>(0x46dd794e),
            TESTED_EXTEND(0, buf, sizeof(buf)));

  for (int i = 0; i < 32; ++i)
    buf[i] = static_cast<uint8_t>(31 - i);
  EXPECT_EQ(static_cast<uint32_t>(0x113fdb5c),
            TESTED_EXTEND(0, buf, sizeof(buf)));

  uint8_t data[48] = {
      0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00,
      0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x18, 0x28, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  };
  EXPECT_EQ(static_cast<uint32_t>(0xd9963a56),
            TESTED_EXTEND(0, data, sizeof(data)));
}

TEST(TESTED_EXTEND_CASE_NAME, HelloWorld) {
  const uint8_t* hello_space_world =
      reinterpret_cast<const uint8_t*>("hello world");
  const uint8_t* hello_space = reinterpret_cast<const uint8_t*>("hello ");
  const uint8_t* world = reinterpret_cast<const uint8_t*>("world");

  EXPECT_EQ(TESTED_EXTEND(0, hello_space_world, 11),
            TESTED_EXTEND(TESTED_EXTEND(0, hello_space, 6), world, 5));
}

TEST(TESTED_EXTEND_CASE_NAME, BufferSlicing) {
  uint8_t buffer[48] = {
      0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00,
      0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x18, 0x28, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  };

  for (size_t i = 0; i < 48; ++i) {
    for (size_t j = i + 1; j <= 48; ++j) {
      uint32_t crc = 0;

      if (i > 0) crc = TESTED_EXTEND(crc, buffer, i);
      crc = TESTED_EXTEND(crc, buffer + i, j - i);
      if (j < 48) crc = TESTED_EXTEND(crc, buffer + j, 48 - j);

      EXPECT_EQ(static_cast<uint32_t>(0xd9963a56), crc);
    }
  }
}

TEST(TESTED_EXTEND_CASE_NAME, LargeBufferSlicing) {
  uint8_t buffer[2048];
  for (size_t i = 0; i < 2048; i++)
    buffer[i] = static_cast<uint8_t>(3 * i * i + 7 * i + 11);

  for (size_t i = 0; i < 2048; ++i) {
    for (size_t j = i + 1; j <= 2048; ++j) {
      uint32_t crc = 0;

      if (i > 0) crc = TESTED_EXTEND(crc, buffer, i);
      crc = TESTED_EXTEND(crc, buffer + i, j - i);
      if (j < 2048) crc = TESTED_EXTEND(crc, buffer + j, 2048 - j);

      EXPECT_EQ(static_cast<uint32_t>(0x36dcc753), crc);
    }
  }
}

}  // namespace crc32c

#undef TESTED_EXTEND_CASE_NAME
#undef TESTED_EXTEND_CASE_NAMER
#undef TESTED_EXTEND_PASTER

#endif  // CRC32C_CRC32C_EXTEND_UNITTESTS_H_
