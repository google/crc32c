// Copyright (c) 2017 The CRC32C Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "crc32c/crc32c.h"

#include <cstddef>
#include <cstdint>

#include "gtest/gtest.h"

TEST(Crc32CTest, CRC32C) {
  // From rfc3720 section B.4.
  std::uint8_t buf[32];

  memset(buf, 0, sizeof(buf));
  EXPECT_EQ(static_cast<std::uint32_t>(0x8a9136aa), CRC32C(buf, sizeof(buf)));

  memset(buf, 0xff, sizeof(buf));
  EXPECT_EQ(static_cast<std::uint32_t>(0x62a8ab43), CRC32C(buf, sizeof(buf)));

  for (std::size_t i = 0; i < 32; i++) {
    buf[i] = i;
  }
  EXPECT_EQ(static_cast<std::uint32_t>(0x46dd794e), CRC32C(buf, sizeof(buf)));

  for (std::size_t i = 0; i < 32; i++) {
    buf[i] = 31 - i;
  }
  EXPECT_EQ(static_cast<std::uint32_t>(0x113fdb5c), CRC32C(buf, sizeof(buf)));

  std::uint8_t data[48] = {
    0x01, 0xc0, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x14, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x04, 0x00,
    0x00, 0x00, 0x00, 0x14,
    0x00, 0x00, 0x00, 0x18,
    0x28, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,
  };
  EXPECT_EQ(static_cast<std::uint32_t>(0xd9963a56), CRC32C(data, sizeof(data)));
}

TEST(CRC32CTest, CRC32C_Extend) {
  const std::uint8_t* hello_space_world =
      reinterpret_cast<const std::uint8_t*>("hello world");
  const std::uint8_t* hello_space =
      reinterpret_cast<const std::uint8_t*>("hello ");
  const std::uint8_t* world =
      reinterpret_cast<const std::uint8_t*>("world");

  EXPECT_EQ(
      CRC32C(hello_space_world, 11),
      CRC32C_Extend(CRC32C(hello_space, 6), world, 5));
}

#define TESTED_EXTEND CRC32C_Extend
#include "./crc32c_extend_unittests.h"
#undef TESTED_EXTEND
