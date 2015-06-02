// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#include "sliding_window_maximum_benchmark.h"

#include <stdlib.h>
#include <i386/limits.h>
#include <algorithm>

#include "benchmark.h"
#include "deque_method.h"
#include "naive.h"
#include "track_back.h"
#include "track_back_optimized.h"
#include "three_phase.h"
#include "two_phase.h"
#include "four_phase.h"

class BaseTestData {

  int *_array;
  size_t _windowSize;
  size_t _length;

public:

  const int *array() const {
    return _array;
  }

  size_t windowSize() const {
    return _windowSize;
  }

  size_t length() const {
    return _length;
  }

  BaseTestData(unsigned int seed, float ratio, float size) {
    srand(seed);
    _length = (size_t const) size;
    _array = (int *) malloc(_length * sizeof(int));
    for (size_t i = 0; i < _length; i++)
      _array[i] = rand() % INT_MAX;

    _windowSize = std::max((size_t) 1, (size_t) (size * ratio));
  }

  ~BaseTestData() {
    free(_array);
  }

};

class SpeedTestData : public BaseTestData {
public:
  SpeedTestData(unsigned int seed, float size) : BaseTestData(seed, 0.3, size) {
  }
};

class RatioTestData : public BaseTestData {
public:
  RatioTestData(unsigned int seed, float ratio) : BaseTestData(seed, ratio, 8000000) {
  }
};

class Output0 {
  int *_out;
  size_t _length;

public:
  int *out() const {
    return _out;
  }

  Output0(const BaseTestData *testData) {
    _length = testData->length();
    _out = (int *) calloc(testData->length(), sizeof(int));
  }

  ~Output0() {
    free(_out);
  }

  bool operator<(Output0 const &right) const {
    if (_length < right._length)
      return true;
    if (_length > right._length)
      return false;
    for (int i = 0; i < _length; i++) {
      if (_out[i] < right._out[i]) {
        return true;
      } else if (_out[i] > right._out[i]) {
        return false;
      }
    }
    return false;
  }
};

template <typename TestData>
class SlidingWindowMaximumBenchmark : public Benchmark<TestData, Output0> {

protected:
  void addMethod(const char* name,
                 void (*method)(const int *, size_t, size_t, int *)) {
    Benchmark<TestData, Output0>::addMethod(name, [=](const TestData &data, Output0 *output) {
        method(data.array(), data.length(), data.windowSize(), output->out());
    });
  }
  
public:

  SlidingWindowMaximumBenchmark() {
    using namespace slidingWindowMaximum;
    addMethod("dequeMethod", dequeMethod);
    addMethod("twoPhase", twoPhase);
    addMethod("threePhase", threePhase);
    addMethod("fourPhase", fourPhase);
    addMethod("trackBack", trackBack);
    addMethod("trackBackOptimized", trackBackOptimized);
  }
};

class SpeedBenchmark : public SlidingWindowMaximumBenchmark<SpeedTestData> {

public:
  SpeedBenchmark() {
    using namespace slidingWindowMaximum;
    addMethod("naive", naive);
  }
};

class RatioBenchmark : public SlidingWindowMaximumBenchmark<RatioTestData> {};

void slidingWindowMaximumBenchmark() {
  if (true) {
    SpeedBenchmark benchmark;
    int samples = 30;
    int min = 1;
    int max = 1000000;
    int distribution = 4;
    int cap = 12000000;
    char const *xlabel = "Bytes";
    bool rounded = true;
    benchmark.run(samples, min, max, distribution, rounded, cap, xlabel);
  } else {
    RatioBenchmark benchmark;
    int samples = 30;
    int min = 0;
    int max = 1;
    int distribution = 1;
    bool rounded = false;
    int cap = 400000000;
    char const *xlabel = "Window size ratio";
    benchmark.run(samples, min, max, distribution, rounded, cap, xlabel);
  }
}