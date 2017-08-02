#ifndef CRC32C_CRC32C_INTERNAL_H_
#define CRC32C_CRC32C_INTERNAL_H_

// Internal functions that may change between releases.

#include "./crc32c.h"

// Software implementation in crc32c_portable.cc
uint32_t CRC32C_Extend_Portable(uint32_t crc, const char* data, size_t count);

#endif  // CRC32C_CRC32C_INTERNAL_H_
