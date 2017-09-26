// Copyright 2017 The CRC32C Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "./crc32c_arm64.h"

#if HAVE_ARM64_CRC32C

#define TESTED_EXTEND ExtendArm64
#include "./crc32c_extend_unittests.h"
#undef TESTED_EXTEND

#endif  // HAVE_ARM64_CRC32C
