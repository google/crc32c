// Copyright 2019 The CRC32C Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include "crc32c/crc32c.h"

#include <string>

// Entry point for LibFuzzer.
extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
	if (size == 0)
		return 0;

	// Hash contents of raw byte buffer
	uint32_t hash = crc32c::Crc32c(data, size);
	// Extend
	hash = crc32c::Extend(hash, data, size);

	// Hash contents of std::string
	std::string dataString(data, data + size - 1);
	hash = crc32c::Crc32c(dataString);
	return 0;
}