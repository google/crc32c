# Copyright 2017 The CRC32C Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file. See the AUTHORS file for names of contributors.

"""Defines the function to load the Crc32c dependencies."""

def crc32c_deps():
    """Load dependencies needed to compile and test the Crc32c library.

    Application developers can call this function from their WORKSPACE file
    to obtain all the necessary dependencies for crc32c. This function only
    loads dependencies that have not been previously loaded, allowing
    application developers to override the version of the dependencies they
    want to use.
    """

    # Load a newer version of google test than what gRPC does.
    if "com_google_googletest" not in native.existing_rules():
        native.http_archive(
            name = "com_google_googletest",
            strip_prefix = "googletest-release-1.8.1",
            url = "https://github.com/google/googletest/archive/release-1.8.1.tar.gz",
            sha256 = "9bf1fe5182a604b4135edc1a425ae356c9ad15e9b23f9f12a02e80184c3a249c",
        )
