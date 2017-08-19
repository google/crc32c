// Copyright (c) 2017 The CRC32C Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "./crc32c_read_le.h"

#include <cstddef>
#include <cstdint>

#include "gtest/gtest.h"

namespace crc32c {

TEST(Uint32LETest, ReadUint32LE) {
  // little-endian 0x12345678
  alignas(4) std::uint8_t bytes[] = {0x78, 0x56, 0x34, 0x12};

  ASSERT_EQ(
      (reinterpret_cast<uintptr_t>(bytes) >> 2) << 2,
      reinterpret_cast<uintptr_t>(bytes)) << "Stack array is not aligned";
  EXPECT_EQ(
      static_cast<uint32_t>(0x12345678),
      ReadUint32LE(bytes));
}

TEST(Uint32LETest, ReadUint64LE) {
  // little-endian 0x123456789ABCDEF0
  alignas(8) std::uint8_t bytes[] =
      {0xF0, 0xDE, 0xBC, 0x9A, 0x78, 0x56, 0x34, 0x12};

  ASSERT_EQ(
      (reinterpret_cast<uintptr_t>(bytes) >> 3) << 3,
      reinterpret_cast<uintptr_t>(bytes)) << "Stack array is not aligned";
  EXPECT_EQ(
      static_cast<uint64_t>(0x123456789ABCDEF0),
      ReadUint64LE(bytes));
}

}  // namespace crc32c
