# CRC32C

This project collects a few CRC32C implementations under an umbrella that
dispatches to a suitable implementation based on the host computer's hardware
capabilities.

CRC32C is specified as the CRC that uses the iSCSI polynomial in
[RFC 3720](https://tools.ietf.org/html/rfc3720#section-12.1). The polynomial was
introduced by G. Castagnoli, S. Braeuer and M. Herrmann. CRC32C is used in
software such as Btrfs, ext4, Ceph and leveldb.

## Building

```bash
make all CXXFLAGS=-O2
make install
```

On a desktop with a modern compiler, the following will produce a faster build.

```bash
make all CXXFLAGS=-O2 HAVE_SSE42=1
```

## Development

The following commands will come in handy.

* `./out/crc32c_tests` - runs tests on desktop
* `./out/crc32c_bench` - runs benchmarks on desktop
* `ndk-build` - build for Android

Run the tests on Android using the following commands.

```bash
ARCH=arm64-v8a
adb push obj/local/$ARCH/crc32c_tests /data/local/tmp/
adb shell /data/local/tmp/crc32c_tests
```

Run the benchmarks on Android using the following commands.

```bash
ARCH=arm64-v8a
adb push obj/local/$ARCH/crc32c_bench /data/local/tmp/
adb shell /data/local/tmp/crc32c_bench
```
