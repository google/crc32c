# The output directory.
OUTDIR = out

# The installation prefix.
prefix = /usr/local

include googletest.mk

HEADERS = $(wildcard crc32c/*.h)
SOURCES = $(wildcard crc32c/*.cc)
OBJECTS = $(patsubst crc32c/%.cc, $(OUTDIR)/%.o, $(SOURCES))
TEST_OBJECTS = $(filter %_test.o, $(OBJECTS))
LIBRARY_OBJECTS = $(filter-out crc32c/crc32c_bench.cc, \
		$(filter-out $(TEST_OBJECTS), $(OBJECTS)))

ifeq ($(HAVE_SSE42), 1)
CXXFLAGS += -DHAVE_SSE42
SSE42FLAGS = -msse4.2
else
SSE42FLAGS =
endif

OPTFLAGS = -O2
CXXFLAGS += $(OPTFLAGS)

$(OUTDIR)/%_sse42.o: crc32c/%_sse42.cc $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(SSE42FLAGS) -c $< -o $@

$(OUTDIR)/%.o: crc32c/%.cc $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(OUTDIR)/libcrc32c.a: $(LIBRARY_OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

$(OUTDIR)/crc32c_bench: $(OUTDIR)/libcrc32c.a out/crc32c_bench.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

$(OUTDIR)/%_test.o: crc32c/%_test.cc $(HEADERS) $(GTEST_HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(OUTDIR)/crc32c_tests: $(OUTDIR)/gtest_main.a $(OUTDIR)/libcrc32c.a \
                        $(TEST_OBJECTS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

# House-keeping build targets.

all : $(OUTDIR)/crc32c_tests $(OUTDIR)/crc32c_bench

install: $(OUTDIR)/libcrc32c.a
	install -m 0755 $(OUTDIR)/libcrc32c.a $(prefix)/lib
	install -m 0644 crc32c/crc32c.h $(prefix)/include

clean :
	rm -f $(OUTDIR)/*
