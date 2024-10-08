// Copyright 2017 The CRC32C Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "./crc32c_read_le.h"

#include <cstddef>
#include <cstdint>

#include "gtest/gtest.h"

#include "./crc32c_round_up.h"

namespace crc32c {

TEST(Crc32CReadLETest, ReadUint16LE) {
  // little-endian 0x1234
  uint8_t bytes[] = {0x34, 0x12};

  EXPECT_EQ(uint16_t{0x1234}, ReadUint16LE(bytes));
}

TEST(Crc32CReadLETest, ReadUint32LE) {
  // little-endian 0x12345678
  uint8_t bytes[] = {0x78, 0x56, 0x34, 0x12};

  EXPECT_EQ(uint32_t{0x12345678}, ReadUint32LE(bytes));
}

TEST(Crc32CReadLETest, ReadUint64LE) {
  // little-endian 0x123456789ABCDEF0
  uint8_t bytes[] = {0xF0, 0xDE, 0xBC, 0x9A, 0x78, 0x56, 0x34, 0x12};

  EXPECT_EQ(uint64_t{0x123456789ABCDEF0}, ReadUint64LE(bytes));
}

}  // namespace crc32c
