// Copyright (c) 2011 The LevelDB Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file. See the AUTHORS file for names of contributors.

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>

#include <string>

#include "./crc32c.h"
#include "./crc32c_internal.h"
#include "./crc32c_sse42.h"


// Comma-separated list of operations to run in the specified order
//   Actual benchmarks:
//      portable_256     -- portable C implementation, 256 bytes
//      portable_4k      -- portable C implementation, 4KB
//      portable_64k     -- portable C implementation, 64KB
//      public_256       -- the method exposed to the public API, 256 bytes
//      public_4k        -- the method exposed to the public API, 4KB
//      public_64k       -- the method exposed to the public API, 64KB
//      sse42_256        -- accelerated with Intel SSE 4.2, 256 bytes
//      sse42_4k         -- accelerated with Intel SSE 4.2, 4KB
//      sse42_64k        -- accelerated with Intel SSE 4.2, 64KB
static const char* FLAGS_benchmarks =
    "portable_256,"
    "portable_4k,"
    "portable_64k,"
    "public_256,"
    "public_4k,"
    "public_64k,"
    "sse42_256,"
    "sse42_4k,"
    "sse42_64k,"
    ;

// Number of megabytes to go over for benchmarking.
static int FLAGS_megabytes = 1024;

#if defined(__linux)
static std::string TrimSpace(const std::string& s) {
  size_t start = 0;
  while (start < s.size() && isspace(s[start])) {
    start++;
  }
  size_t limit = s.size();
  while (limit > start && isspace(s[limit-1])) {
    limit--;
  }
  return std::string(s.data() + start, limit - start);
}
#endif

static void AppendWithSpace(std::string* str, const std::string& msg) {
  if (msg.empty()) return;
  if (!str->empty()) {
    str->push_back(' ');
  }
  str->append(msg);
}

class Stats {
 private:
  double start_;
  double finish_;
  double seconds_;
  int done_;
  int next_report_;
  int64_t bytes_;
  double last_op_finish_;
  std::string message_;

 public:
  Stats() { Start(); }

  static uint64_t NowMicros() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return static_cast<uint64_t>(tv.tv_sec) * 1000000 + tv.tv_usec;
  }

  void Start() {
    next_report_ = 100;
    last_op_finish_ = start_;
    done_ = 0;
    bytes_ = 0;
    seconds_ = 0;
    start_ = Stats::NowMicros();
    finish_ = start_;
    message_.clear();
  }

  void Stop() {
    finish_ = Stats::NowMicros();
    seconds_ = (finish_ - start_) * 1e-6;
  }

  void AddMessage(const std::string& msg) {
    AppendWithSpace(&message_, msg);
  }

  void FinishedSingleOp() {
    done_++;
    if (done_ >= next_report_) {
      if      (next_report_ < 1000)   next_report_ += 100;
      else if (next_report_ < 5000)   next_report_ += 500;
      else if (next_report_ < 10000)  next_report_ += 1000;
      else if (next_report_ < 50000)  next_report_ += 5000;
      else if (next_report_ < 100000) next_report_ += 10000;
      else if (next_report_ < 500000) next_report_ += 50000;
      else                            next_report_ += 100000;
      fprintf(stderr, "... finished %d ops%30s\r", done_, "");
      fflush(stderr);
    }
  }

  void AddBytes(int64_t n) {
    bytes_ += n;
  }

  void Report(const std::string& name) {
    // Pretend at least one op was done in case we are running a benchmark
    // that does not call FinishedSingleOp().
    if (done_ < 1) done_ = 1;

    std::string extra;
    if (bytes_ > 0) {
      // Rate is computed on actual elapsed time, not the sum of per-thread
      // elapsed times.
      double elapsed = (finish_ - start_) * 1e-6;
      char rate[100];
      snprintf(rate, sizeof(rate), "%6.1f MB/s",
               (bytes_ / 1048576.0) / elapsed);
      extra = rate;
    }
    AppendWithSpace(&extra, message_);

    fprintf(stdout, "%-12s : %11.3f micros/op;%s%s\n",
            name.c_str(),
            seconds_ * 1e6 / done_,
            (extra.empty() ? "" : " "),
            extra.c_str());
    fflush(stdout);
  }
};

class Benchmark {
 private:
  size_t block_size_;

  void PrintWarnings() {
#if defined(__GNUC__) && !defined(__OPTIMIZE__)
    fprintf(
        stdout,
        "WARNING: Optimization is disabled: benchmarks unnecessarily slow\n");
#endif
  }

  void PrintEnvironment() {
#if defined(__linux)
    time_t now = time(NULL);
    fprintf(stderr, "Date:       %s", ctime(&now));  // ctime() adds newline

    FILE* cpuinfo = fopen("/proc/cpuinfo", "r");
    if (cpuinfo != NULL) {
      char line[1000];
      int num_cpus = 0;
      std::string cpu_type;
      std::string cache_size;
      while (fgets(line, sizeof(line), cpuinfo) != NULL) {
        const char* sep = strchr(line, ':');
        if (sep == NULL) {
          continue;
        }
        std::string key = TrimSpace(std::string(line, sep - 1 - line));
        std::string val = TrimSpace(std::string(sep + 1));
        if (key == "model name") {
          ++num_cpus;
          cpu_type = val;
        } else if (key == "cache size") {
          cache_size = val;
        }
      }
      fclose(cpuinfo);
      fprintf(stderr, "CPU:        %d * %s\n", num_cpus, cpu_type.c_str());
      fprintf(stderr, "CPUCache:   %s\n", cache_size.c_str());
    }
#endif
  }

 public:
  Benchmark() {
  }

  void Run() {
    const char* benchmarks = FLAGS_benchmarks;
    while (benchmarks != NULL) {
      const char* sep = strchr(benchmarks, ',');
      std::string name;
      if (sep == NULL) {
        name = benchmarks;
        benchmarks = NULL;
      } else {
        name = std::string(benchmarks, sep - benchmarks);
        benchmarks = sep + 1;
      }

      // Reset parameters that may be overridden below
      block_size_ = 1;

      void (Benchmark::*method)(Stats*) = NULL;

      if (name == "public_256") {
        method = &Benchmark::Public;
        block_size_ = 256;
      } else if (name == "public_4k") {
        method = &Benchmark::Public;
        block_size_ = 4096;
      } else if (name == "public_64k") {
        method = &Benchmark::Public;
        block_size_ = 65536;
      } else if (name == "portable_256") {
        method = &Benchmark::Portable;
        block_size_ = 256;
      } else if (name == "portable_4k") {
        method = &Benchmark::Portable;
        block_size_ = 4096;
      } else if (name == "portable_64k") {
        method = &Benchmark::Portable;
        block_size_ = 65536;
      } else if (name == "sse42_256") {
        method = &Benchmark::SSE42;
        block_size_ = 256;
      } else if (name == "sse42_4k") {
        method = &Benchmark::SSE42;
        block_size_ = 4096;
      } else if (name == "sse42_64k") {
        method = &Benchmark::SSE42;
        block_size_ = 65536;
      }

      if (method != NULL) {
        RunBenchmark(name, method);
      }
    }
  }

 private:
  void RunBenchmark(const std::string& name,
                    void (Benchmark::*method)(Stats*)) {
    Stats stats;
    stats.Start();
    (this->*method)(&stats);
    stats.Stop();
    stats.Report(name);
  }

  void Public(Stats* stats) {
    std::string data(block_size_, 'x');
    const char* block_buffer = data.data();

    int64_t bytes = 0;
    int64_t benchmark_size = FLAGS_megabytes * 1024 * 1024;
    uint32_t crc = 0;
    while (bytes < benchmark_size) {
      crc = CRC32C_Extend(0, block_buffer, block_size_);
      stats->FinishedSingleOp();
      bytes += block_size_;
    }
    // Print so result is not dead
    fprintf(stderr, "... crc=0x%x\r", static_cast<unsigned int>(crc));

    stats->AddBytes(bytes);
  }

  void Portable(Stats* stats) {
    std::string data(block_size_, 'x');
    const char* block_buffer = data.data();

    int64_t bytes = 0;
    int64_t benchmark_size = FLAGS_megabytes * 1024 * 1024;
    uint32_t crc = 0;
    while (bytes < benchmark_size) {
      crc = CRC32C_Extend_Portable(0, block_buffer, block_size_);
      stats->FinishedSingleOp();
      bytes += block_size_;
    }
    // Print so result is not dead
    fprintf(stderr, "... crc=0x%x\r", static_cast<unsigned int>(crc));

    stats->AddBytes(bytes);
  }

  void SSE42(Stats* stats) {
#if defined(HAVE_SSE42)
    if (!CRC32C_CanUseSSE42()) {
      stats->AddMessage("(no SSE4.2 CPU support)");
      return;
    }

    std::string data(block_size_, 'x');
    const char* block_buffer = data.data();

    int64_t bytes = 0;
    int64_t benchmark_size = FLAGS_megabytes * 1024 * 1024;
    uint32_t crc = 0;
    while (bytes < benchmark_size) {
      crc = CRC32C_Extend_SSE42(0, block_buffer, block_size_);
      stats->FinishedSingleOp();
      bytes += block_size_;
    }
    // Print so result is not dead
    fprintf(stderr, "... crc=0x%x\r", static_cast<unsigned int>(crc));

    stats->AddBytes(bytes);
#else  // !defined(HAVE_SSE42)
  stats->AddMessage("(no SSE4.2 compiler support)");
#endif  // defined(HAVE_SSE42)
  }
};

// Needed for Android's NDK.
extern "C" int main(int argc, char** argv);

int main(int argc, char** argv) {
  for (int i = 1; i < argc; i++) {
    int n;
    char junk;
    if (!strncmp(argv[i], "--benchmarks=", strlen("--benchmarks="))) {
      FLAGS_benchmarks = argv[i] + strlen("--benchmarks=");
    } else if (sscanf(argv[i], "--megabytes=%d%c", &n, &junk) == 1) {
      FLAGS_megabytes = n;
    } else {
      fprintf(stderr, "Invalid flag '%s'\n", argv[i]);
      exit(1);
    }
  }

  Benchmark benchmark;
  benchmark.Run();
  return 0;
}
