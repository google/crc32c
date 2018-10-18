# Copyright 2018 The CRC32C Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file. See the AUTHORS file for names of contributors.

package(default_visibility = ["//visibility:public"])

licenses(["notice"])  # BSD

load(":crc32c_arm64.bzl", "crc32c_arm64_HDRS", "crc32c_arm64_SRCS")
load(":crc32c_sse42.bzl", "crc32c_sse42_HDRS", "crc32c_sse42_SRCS")
load(":crc32c.bzl", "crc32c_HDRS", "crc32c_SRCS")

config_setting(
    name = "windows",
    values = {"cpu": "x64_windows"},
    visibility = ["//visibility:public"],
)

config_setting(
    name = "linux_x86_64",
    values = {"cpu": "k8"},
    visibility = ["//visibility:public"],
)

config_setting(
    name = "darwin",
    values = {"cpu": "darwin"},
    visibility = ["//visibility:public"],
)

sse42_copts = select({
    ":windows": ["/arch:AVX"],
    ":linux_x86_64": ["-msse4.2"],
    ":darwin": ["-msse4.2"],
    "//conditions:default": [],
})

sse42_enabled = select({
    ":windows": "1",
    ":linux_x86_64": "1",
    ":darwin": "1",
    "//conditions:default": "0",
})

genrule(
    name = "generate_config",
    srcs = ["src/crc32c_config.h.in"],
    outs = ["crc32c/crc32c_config.h"],
    cmd = """
sed -e 's/#cmakedefine01/#define/' \
    -e 's/ BYTE_ORDER_BIG_ENDIAN/ BYTE_ORDER_BIG_ENDIAN 0/' \
    -e 's/ HAVE_BUILTIN_PREFETCH/ HAVE_BUILTIN_PREFETCH 0/' \
    -e 's/ HAVE_MM_PREFETCH/ HAVE_MM_PREFETCH 0/' \
    -e 's/ HAVE_SSE42/ HAVE_SSE42 1/' \
    -e 's/ HAVE_ARM64_CRC32C/ HAVE_ARM64_CRC32C 0/' \
    -e 's/ HAVE_STRONG_GETAUXVAL/ HAVE_STRONG_GETAUXVAL 0/' \
    -e 's/ HAVE_WEAK_GETAUXVAL/ HAVE_WEAK_GETAUXVAL 0/' \
    -e 's/ CRC32C_TESTS_BUILT_WITH_GLOG/ CRC32C_TESTS_BUILT_WITH_GLOG 0/' \
    < $< > $@
""",
)

cc_library(
    name = "crc32c",
    srcs = crc32c_SRCS + crc32c_sse42_SRCS + crc32c_arm64_SRCS,
    hdrs = crc32c_HDRS + ["crc32c/crc32c_config.h"],
    deps = [],
    includes = ["include"],
    copts = sse42_copts,
)

load(":crc32c_test.bzl", "crc32c_test_sources")
cc_test(
    name = "crc32c_test",
    srcs = crc32c_test_sources,
    deps = [
        ":crc32c",
        "@com_google_googletest//:gtest",
        "@com_google_googletest//:gtest_main",
    ],
)

exports_files([
    "LICENSE",
])
