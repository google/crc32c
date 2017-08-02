// Copyright (c) 2017 The CRC32C Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef CRC32C_CRC32C_H_
#define CRC32C_CRC32C_H_

// The API exported by the CRC32C project.

#include <cstddef>
#include <cstdint>

// Extends "crc" with the CRC32 of "count" bytes in the buffer pointed by
// "data".
std::uint32_t CRC32C_Extend(
    std::uint32_t crc, const std::uint8_t* data, std::size_t count);

// Computes the CRC32C of "count" bytes in the buffer pointed by "data".
inline std::uint32_t CRC32C(const std::uint8_t* data, std::size_t count) {
  return CRC32C_Extend(0, data, count);
}

#endif  // CRC32C_CRC32C_H_
