#include "./crc32c.h"

#include "gtest/gtest.h"

TEST(Crc32CTest, CRC32C) {
  // From rfc3720 section B.4.
  char buf[32];

  memset(buf, 0, sizeof(buf));
  EXPECT_EQ(0x8a9136aaUL, CRC32C(buf, sizeof(buf)));

  memset(buf, 0xff, sizeof(buf));
  EXPECT_EQ(0x62a8ab43UL, CRC32C(buf, sizeof(buf)));

  for (int i = 0; i < 32; i++) {
    buf[i] = i;
  }
  EXPECT_EQ(0x46dd794eUL, CRC32C(buf, sizeof(buf)));

  for (int i = 0; i < 32; i++) {
    buf[i] = 31 - i;
  }
  EXPECT_EQ(0x113fdb5cUL, CRC32C(buf, sizeof(buf)));

  unsigned char data[48] = {
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
  EXPECT_EQ(
      0xd9963a56,
      CRC32C(reinterpret_cast<char*>(data), sizeof(data)));
}

TEST(CRC32CTest, CRC32C_Extend) {
  EXPECT_EQ(
      CRC32C("hello world", 11),
      CRC32C_Extend(CRC32C("hello ", 6), "world", 5));
}

#define TESTED_EXTEND CRC32C_Extend
#include "./crc32c_extend_tests.h"
#undef TESTED_EXTEND
