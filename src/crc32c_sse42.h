// Copyright (c) 2017 The CRC32C Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef CRC32C_CRC32C_SSE42_H_
#define CRC32C_CRC32C_SSE42_H_

// X86-specific code.

#include <cstddef>
#include <cstdint>

#include "crc32c/crc32c_config.h"

#if defined(HAVE_SSE42)

// SSE4.2-accelerated implementation in crc32c_sse42.cc
std::uint32_t CRC32C_Extend_SSE42(
    std::uint32_t crc, const std::uint8_t* data, std::size_t count);

#endif  // defined(HAVE_SSE42)

#endif  // CRC32C_CRC32C_SSE42_H_
