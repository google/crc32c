// Copyright (c) 2017 The CRC32C Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "./crc32c_sse42.h"

// In a separate source file to allow this accelerated CRC32C function to be
// compiled with the appropriate compiler flags to enable SSE4.2 instructions.

// This implementation is based on https://github.com/google/leveldb/pull/309.

#include <cstddef>
#include <cstdint>

#include "crc32c/crc32c_config.h"
#include "./crc32c_internal.h"
#include "./crc32c_read_le.h"
#include "./crc32c_round_up.h"

#if defined(HAVE_SSE42)

#if defined(_MSC_VER)
#include <intrin.h>
#else  // !defined(_MSC_VER)
#include <nmmintrin.h>
#endif  // defined(_MSC_VER)

namespace crc32c {

// For further improvements see Intel publication at:
// http://download.intel.com/design/intarch/papers/323405.pdf
uint32_t ExtendSse42(uint32_t crc, const uint8_t* data, size_t size) {
  const uint8_t* p = data;
  const uint8_t* e = data + size;
  uint32_t l = crc ^ kCRC32Xor;

#define STEP1 do {                              \
    l = _mm_crc32_u8(l, *p++);                  \
} while (0)
#define STEP4 do {                              \
    l = _mm_crc32_u32(l, ReadUint32LE(p));      \
    p += 4;                                     \
} while (0)
#define STEP8 do {                              \
    l = _mm_crc32_u64(l, ReadUint64LE(p));      \
    p += 8;                                     \
} while (0)

  if (size > 16) {
    // Point x at first 8-byte aligned byte in the buffer. This must be inside
    // the buffer, as long as the buffer is 8 bytes or larger.
    const uint8_t* x = RoundUp<8>(p);
    // Process bytes until p is 8-byte aligned.
    while (p != x) {
      STEP1;
    }

    // Process 8 bytes at a time.
    while (e - p >= 8) {
      STEP8;
    }
    // Process 4 bytes at a time.
    if (e - p >= 4) {
      STEP4;
    }
  }
  // Process the last few bytes.
  while (p != e) {
    STEP1;
  }
#undef STEP8
#undef STEP4
#undef STEP1
  return l ^ kCRC32Xor;
}

}  // namespace crc32c

#endif  // defined(HAVE_SSE42)
