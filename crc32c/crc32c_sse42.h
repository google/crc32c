#ifndef CRC32C_CRC32C_SSE42_H_
#define CRC32C_CRC32C_SSE42_H_

// X86-specific code.

#if defined(HAVE_SSE42)

#include "./crc32c.h"

// If the compiler supports SSE4.2, it definitely supports X86.

#if defined(_MSC_VER)
#include <intrin.h>
static inline bool CRC32C_CanUseSSE42() {
  int cpu_info[4];
  __cpuid(cpu_info, 1);
  return (cpu_info[2] & (1 << 20)) != 0;
}
#elif defined(__GNUC__)
#include <cpuid.h>
#include <nmmintrin.h>
static inline bool CRC32C_CanUseSSE42() {
  unsigned int eax, ebx, ecx, edx;
  __get_cpuid(1, &eax, &ebx, &ecx, &edx);
  return (ecx & (1 << 20)) != 0;
}
#else  // !defined(_MSC_VER) && !defined(__GNUC__)
static inline bool IsSSE42Available() {
  return false;
}
#endif  // defined(_MSC_VER)

// SSE4.2-accelerated implementation in crc32c_sse42.cc
uint32_t CRC32C_Extend_SSE42(uint32_t crc, const char* data, size_t count);

#endif  // defined(HAVE_SSE42)

#endif  // CRC32C_CRC32C_SSE42_H_
