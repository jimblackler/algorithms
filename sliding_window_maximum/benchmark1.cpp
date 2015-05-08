// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#include <stdlib.h>
#include <i386/limits.h>
#include "benchmark.h"
#include "naive.h"
#include "track_back.h"
#include "track_back_optimized.h"

class TestData0 {

  int *_array;
  size_t _windowSize;
  size_t _length;

public:

  int x() const {
    return (int) _length;
  }

  const int *array() const {
    return _array;
  }

  size_t windowSize() const {
    return _windowSize;
  }

  size_t length() const {
    return _length;
  }

  TestData0(unsigned int seed, float t) {
    srand(seed);
    _length = (size_t const) (t * 200000 + 1);
    _array = (int *) malloc(_length * sizeof(int));
    for (size_t i = 0; i < _length; i++)
      _array[i] = rand() % INT_MAX;

    _windowSize = _length / 10;
  }

  ~TestData0() {
    free(_array);
  }

};

class Output0 {
  int *_out;
  size_t _length;

public:
  int *out() const {
    return _out;
  }

  Output0(const TestData0 *testData) {
    _length = testData->length();
    _out = (int *) calloc(testData->length(), sizeof(int));
  }

  ~Output0() {
    free(_out);
  }

  bool operator<(Output0 const &right) const {
    for (int i = 0; i < _length; i++)
      if (_out[i] < right._out[i]) {
        return true;
      } else if (_out[i] > right._out[i]) {
        return false;
      }
    return false;
  }
};

class SlidingWindowMaximumBenchmark : public Benchmark<TestData0, Output0> {

public:

  SlidingWindowMaximumBenchmark() {
    addMethod("naive", [](const TestData0 &data, Output0 *output) {
        slidingWindowMaximum::naive(data.array(), data.length(), data.windowSize(), output->out());
    });
    addMethod("trackBack", [](const TestData0 &data, Output0 *output) {
        slidingWindowMaximum::trackBack(data.array(), data.length(), data.windowSize(), output->out());
    });

    addMethod("trackBackOptimized", [](const TestData0 &data, Output0 *output) {
        slidingWindowMaximum::trackBackOptimized(data.array(), data.length(), data.windowSize(), output->out());
    });

  }
};

void benchmark1() {
  SlidingWindowMaximumBenchmark benchmark;
  benchmark.run(100, 4000);
}