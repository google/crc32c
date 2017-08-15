// Copyright (c) 2017 The CRC32C Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#ifndef CRC32C_CRC32C_SSE42_CHECK_H_
#define CRC32C_CRC32C_SSE42_CHECK_H_

// X86-specific code checking the availability of SSE4.2 instructions.

#include <cstddef>
#include <cstdint>

#include "crc32c/crc32c_config.h"

#if defined(HAVE_SSE42)

// If the compiler supports SSE4.2, it definitely supports X86.

#if defined(_MSC_VER)
#include <intrin.h>
static inline bool CRC32C_CanUseSSE42() {
  int cpu_info[4];
  __cpuid(cpu_info, 1);
  return (cpu_info[2] & (1 << 20)) != 0;
}
#else  // !defined(_MSC_VER)
#include <cpuid.h>
static inline bool CRC32C_CanUseSSE42() {
  unsigned int eax, ebx, ecx, edx;
  return __get_cpuid(1, &eax, &ebx, &ecx, &edx) && ((ecx & (1 << 20)) != 0);
}
#endif  // defined(_MSC_VER)

#endif  // defined(HAVE_SSE42)

#endif  // CRC32C_CRC32C_SSE42_CHECK_H_
