// Copyright (c) 2017 The CRC32C Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef CRC32C_CRC32C_INTERNAL_H_
#define CRC32C_CRC32C_INTERNAL_H_

// Internal functions that may change between releases.

#include <cstddef>
#include <cstdint>

#include "crc32c/crc32c.h"

// Software implementation in crc32c_portable.cc
std::uint32_t CRC32C_Extend_Portable(
    std::uint32_t crc, const uint8_t* data, std::size_t count);

#endif  // CRC32C_CRC32C_INTERNAL_H_
