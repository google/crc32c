// Copyright (c) 2017 The CRC32C Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "crc32c/crc32c.h"

#include <cstddef>
#include <cstdint>

#include "./crc32c_internal.h"
#include "./crc32c_sse42.h"
#include "./crc32c_sse42_check.h"

std::uint32_t CRC32C_Extend(
    std::uint32_t crc, const std::uint8_t* data, std::size_t count) {
#ifdef HAVE_SSE42
  static bool can_use_sse42 = CRC32C_CanUseSSE42();
  if (can_use_sse42)
    return CRC32C_Extend_SSE42(crc, data, count);
#endif

  return CRC32C_Extend_Portable(crc, data, count);
}
