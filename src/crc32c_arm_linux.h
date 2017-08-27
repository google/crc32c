// Copyright (c) 2017 The CRC32C Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

// Linux-specific code checking the availability for ARM CRC32C instructions.

#ifndef CRC32C_CRC32C_ARM_LINUX_H_
#define CRC32C_CRC32C_ARM_LINUX_H_

#include <cstddef>
#include <cstdint>

#include "crc32c/crc32c_config.h"

#if defined(HAVE_ARM_CRC32C)

namespace crc32c {

uint32_t ExtendArmLinux(uint32_t crc, const uint8_t* data, size_t count);

}  // namespace crc32c

#endif // defined(HAVE_ARM_CRC32C)

#endif  // CRC32C_CRC32C_ARM_LINUX_H_
