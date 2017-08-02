#ifndef CRC32C_CRC32C_READ_LE_H_
#define CRC32C_CRC32C_READ_LE_H_

#include <stdint.h>
#include <string.h>

#ifdef _WIN32
  #include <winsock2.h>
  #include <sys/types.h>
#elif defined(__APPLE__)
  #include <machine/endian.h>
#elif defined(__OpenBSD__) || defined(__NetBSD__) || defined(__FreeBSD__) || \
      defined(__DragonFly__)
  #include <sys/endian.h>
#else
  #include <endian.h>
#endif

#if defined(__BYTE_ORDER) && !defined(BYTE_ORDER)
#define BYTE_ORDER __BYTE_ORDER
#endif  // defined(__BYTE_ORDER) && !defined(BYTE_ORDER)

#if defined(__LITTLE_ENDIAN) && !defined(LITTLE_ENDIAN)
#define LITTLE_ENDIAN __LITTLE_ENDIAN
#endif  // defined(__LITTLE_ENDIAN) && !defined(LITTLE_ENDIAN)

#if !defined(BYTE_ORDER)
#error "Failed to get a definition for BYTE_ORDER"
#endif  // !defined(BYTE_ORDER)

#if !defined(LITTLE_ENDIAN)
#error "Failed to get a definition for LITTLE_ENDIAN"
#endif  // !defined(LITTLE_ENDIAN)

// Reads a little-endian 32-bit integer from a 32-bit-aligned buffer.
inline uint32_t ReadUint32LE(const uint8_t* buffer) {
#if BYTE_ORDER == LITTLE_ENDIAN
  uint32_t result;
  // This should be optimized to a single instruction.
  memcpy(&result, buffer, sizeof(result));
  return result;
#else  // BYTE_ORDER != LITTLE_ENDIAN
  return ((static_cast<uint32_t>(static_cast<unsigned char>(buffer[0])))
      | (static_cast<uint32_t>(static_cast<unsigned char>(buffer[1])) << 8)
      | (static_cast<uint32_t>(static_cast<unsigned char>(buffer[2])) << 16)
      | (static_cast<uint32_t>(static_cast<unsigned char>(buffer[3])) << 24));
#endif  // BYTE_ORDER == LITTLE_ENDIAN
}

// Reads a little-endian 64-bit integer from a 64-bit-aligned buffer.
inline uint64_t ReadUint64LE(const uint8_t* buffer) {
#if BYTE_ORDER == LITTLE_ENDIAN
  uint64_t result;
  // This should be optimized to a single instruction.
  memcpy(&result, buffer, sizeof(result));
  return result;
#else  // BYTE_ORDER != LITTLE_ENDIAN
  return ((static_cast<uint32_t>(static_cast<unsigned char>(buffer[0])))
      | (static_cast<uint32_t>(static_cast<unsigned char>(buffer[1])) << 8)
      | (static_cast<uint32_t>(static_cast<unsigned char>(buffer[2])) << 16)
      | (static_cast<uint32_t>(static_cast<unsigned char>(buffer[3])) << 24)
      | (static_cast<uint32_t>(static_cast<unsigned char>(buffer[4])) << 32)
      | (static_cast<uint32_t>(static_cast<unsigned char>(buffer[5])) << 40)
      | (static_cast<uint32_t>(static_cast<unsigned char>(buffer[6])) << 48)
      | (static_cast<uint32_t>(static_cast<unsigned char>(buffer[7])) << 56));
#endif  // BYTE_ORDER == LITTLE_ENDIAN
}

#endif  // CRC32C_CRC32C_READ_LE_H_
