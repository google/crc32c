#ifndef CRC32C_CRC32C_EXTEND_TESTS_H_
#define CRC32C_CRC32C_EXTEND_TESTS_H_

#include <stdint.h>
#include <string.h>

#include <gtest/gtest.h>

// Common test cases for all implementations of CRC32C_Extend().
// TESTED_EXTEND should be #defined to the tested function's name.

// TESTED_EXTEND_CASE_NAME ends up with a test case name that depends on
// TESTED_EXTEND.
// The technique is described in http://stackoverflow.com/a/1489985
#define TESTED_EXTEND_PASTER(x,y) x ## y
#define TESTED_EXTEND_CASE_NAMER(x,y) TESTED_EXTEND_PASTER(x,y)
#define TESTED_EXTEND_CASE_NAME TESTED_EXTEND_CASE_NAMER(TESTED_EXTEND, Test)

TEST(TESTED_EXTEND_CASE_NAME, StandardResults) {
  // From rfc3720 section B.4.
  char buf[32];

  memset(buf, 0, sizeof(buf));
  EXPECT_EQ(
      static_cast<uint32_t>(0x8a9136aa), TESTED_EXTEND(0, buf, sizeof(buf)));

  memset(buf, 0xff, sizeof(buf));
  EXPECT_EQ(
      static_cast<uint32_t>(0x62a8ab43), TESTED_EXTEND(0, buf, sizeof(buf)));

  for (int i = 0; i < 32; i++) {
    buf[i] = i;
  }
  EXPECT_EQ(
      static_cast<uint32_t>(0x46dd794e), TESTED_EXTEND(0, buf, sizeof(buf)));

  for (int i = 0; i < 32; i++) {
    buf[i] = 31 - i;
  }
  EXPECT_EQ(
      static_cast<uint32_t>(0x113fdb5c), TESTED_EXTEND(0, buf, sizeof(buf)));

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
      static_cast<uint32_t>(0xd9963a56),
      TESTED_EXTEND(0, reinterpret_cast<char*>(data), sizeof(data)));
}

TEST(TESTED_EXTEND_CASE_NAME, HelloWorld) {
  EXPECT_EQ(
      TESTED_EXTEND(0, "hello world", 11),
      TESTED_EXTEND(TESTED_EXTEND(0, "hello ", 6), "world", 5));
}

TEST(TESTED_EXTEND_CASE_NAME, BufferSlicing) {
  unsigned char buffer[48] = {
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

  for (size_t i = 0; i < 48; i++) {
    for (size_t j = i + 1; j <= 48; j++) {
      uint32_t crc = 0;

      if (i > 0)
        crc = TESTED_EXTEND(crc, reinterpret_cast<char*>(buffer), i);
      crc = TESTED_EXTEND(crc, reinterpret_cast<char*>(buffer + i), j - i);
      if (j < 48)
        crc = TESTED_EXTEND(crc, reinterpret_cast<char*>(buffer + j), 48 - j);

      EXPECT_EQ(static_cast<uint32_t>(0xd9963a56), crc);
    }
  }
}

#undef TESTED_EXTEND_CASE_NAME
#undef TESTED_EXTEND_CASE_NAMER
#undef TESTED_EXTEND_PASTER

#endif  // CRC32C_CRC32C_EXTEND_TESTS_H_
