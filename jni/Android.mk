LOCAL_PATH := $(call my-dir)/..

MY_SOURCES = $(wildcard crc32c/*.cc)
MY_TEST_SOURCES = $(filter %_test.cc, $(MY_SOURCES))
MY_BENCH_SOURCES = $(filter %_bench.cc, $(MY_SOURCES))
MY_LIBRARY_SOURCES = $(filter-out $(MY_BENCH_SOURCES), \
 		$(filter-out $(MY_TEST_SOURCES), $(MY_SOURCES)))

include $(CLEAR_VARS)
LOCAL_MODULE := crc32c
LOCAL_SRC_FILES := $(MY_LIBRARY_SOURCES)
LOCAL_CPPFLAGS := -Wall -Wextra
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := crc32c_bench
LOCAL_SRC_FILES := $(MY_BENCH_SOURCES)
LOCAL_STATIC_LIBRARIES := crc32c
LOCAL_CPPFLAGS := -Wall -Wextra
include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := crc32c_tests
LOCAL_SRC_FILES := $(MY_TEST_SOURCES)
LOCAL_STATIC_LIBRARIES := crc32c googletest_main
LOCAL_CPPFLAGS := -Wall -Wextra
LOCAL_LDFLAGS := -latomic
include $(BUILD_EXECUTABLE)

$(call import-module,third_party/googletest)
