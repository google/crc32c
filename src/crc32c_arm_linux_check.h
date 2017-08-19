// Copyright (c) 2017 The CRC32C Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

// ARM Linux-specific code checking for the availability of CRC32C instructions.

#ifndef CRC32C_CRC32C_ARM_LINUX_CHECK_H_
#define CRC32C_CRC32C_ARM_LINUX_CHECK_H_

// X86-specific code checking for the availability of SSE4.2 instructions.

#include <cstddef>
#include <cstdint>

#include "crc32c/crc32c_config.h"

#if defined(HAVE_ARM_LINUX_CRC32C)

#include <sys/auxv.h>

namespace crc32c {

inline bool CanUseArmLinux() {
  // From 'arch/arm64/include/uapi/asm/hwcap.h' in Linux kernel source code.
  constexpr unsigned long kHwCapCrc32 = 1 << 7;
  unsigned long hwcap = getauxval(AT_HWCAP);
  return (hwcap & kHwCapCrc32) != 0;
}

}  // namespace crc32c

#endif  // defined(HAVE_ARM_LINUX_CRC32C)

#endif  // CRC32C_CRC32C_ARM_LINUX_CHECK_H_
