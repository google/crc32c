#include "./crc32c_internal.h"

#define TESTED_EXTEND CRC32C_Extend_Portable
#include "./crc32c_extend_tests.h"
#undef TESTED_EXTEND
