#ifndef CRC32C_CRC32C_H_
#define CRC32C_CRC32C_H_

// The API exported by the CRC32C project.

#include <cstddef>
#include <cstdint>

// Extends "crc" with the CRC32 of "count" bytes in the buffer pointed by
// "data".
uint32_t CRC32C_Extend(uint32_t crc, const char* data, size_t count);

// Computes the CRC32C of "count" bytes in the buffer pointed by "data".
inline uint32_t CRC32C(const char* data, size_t count) {
  return CRC32C_Extend(0, data, count);
}

#endif  // CRC32C_CRC32C_H_
