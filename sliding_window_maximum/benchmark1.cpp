// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#include "benchmark.h"

#include <math.h>

#include "deque_method.h"
#include "naive.h"
#include "track_back.h"
#include "track_back_optimized.h"
#include "three_phase.h"
#include "two_phase.h"
#include "four_phase.h"

class TestData0 {

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

  TestData0(unsigned int seed, int size) {
    srand(seed);
    _length = (size_t const) size;
    _array = (int *) malloc(_length * sizeof(int));
    for (size_t i = 0; i < _length; i++)
      _array[i] = rand() % INT_MAX;

    _windowSize = _length / 5 + 1;
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
    if (_length < right._length)
      return true;
    if (_length > right._length)
      return false;
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

  void addMethod(const char* name,
                 void (*method)(const int *, size_t, size_t, int *)) {
    Benchmark::addMethod(name, [=](const TestData0 &data, Output0 *output) {
        method(data.array(), data.length(), data.windowSize(), output->out());
    });
  }
  
public:

  SlidingWindowMaximumBenchmark() {
    using namespace slidingWindowMaximum;
    addMethod("naive", naive);
    addMethod("dequeMethod", dequeMethod);
    addMethod("twoPhase", twoPhase);
    addMethod("threePhase", threePhase);
    addMethod("fourPhase", fourPhase);
    addMethod("trackBack", trackBack);
    addMethod("trackBackOptimized", trackBackOptimized);
  }


};

void benchmark1() {
  SlidingWindowMaximumBenchmark benchmark;
  benchmark.run(30, 6000);
}