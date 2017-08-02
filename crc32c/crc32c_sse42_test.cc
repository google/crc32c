#include "./crc32c_sse42.h"

#if defined(HAVE_SSE42)

#define TESTED_EXTEND CRC32C_Extend_SSE42
#include "./crc32c_extend_tests.h"
#undef TESTED_EXTEND

#endif  // defined(HAVE_SSE42)
