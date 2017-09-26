// Copyright 2017 The CRC32C Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "./crc32c_sse42.h"

#if HAVE_SSE42 && (defined(_M_X64) || defined(__x86_64__))

#define TESTED_EXTEND ExtendSse42
#include "./crc32c_extend_unittests.h"
#undef TESTED_EXTEND

#endif  // HAVE_SSE42 && (defined(_M_X64) || defined(__x86_64__))
