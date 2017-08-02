#include "./crc32c.h"
#include "./crc32c_internal.h"
#include "./crc32c_sse42.h"

uint32_t CRC32C_Extend(uint32_t crc, const char* data, size_t count) {
#ifdef HAVE_SSE42
  static bool canUseSSE42 = CRC32C_CanUseSSE42();
  if (canUseSSE42)
    return CRC32C_Extend_SSE42(crc, data, count);
#endif

  return CRC32C_Extend_Portable(crc, data, count);
}
