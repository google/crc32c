// Copyright 2017 The CRC32C Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "./crc32c_sse42.h"

#include "./crc32c_extend_unittests.h"
#include "gtest/gtest.h"

namespace crc32c {

#if HAVE_SSE42 && (defined(_M_X64) || defined(__x86_64__)) && \
    (defined(__SSE4_2__) || defined(__AVX__))

struct Sse42TestTraits {
  static uint32_t Extend(uint32_t crc, const uint8_t* data, size_t count) {
    return ExtendSse42(crc, data, count);
  }
};

INSTANTIATE_TYPED_TEST_SUITE_P(Sse42, ExtendTest, Sse42TestTraits);

#endif  // HAVE_SSE42 && (defined(_M_X64) || defined(__x86_64__)) &&
        // (defined(__SSE4_2__) || defined(__AVX__))

}  // namespace crc32c
