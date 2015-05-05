#include <stdlib.h>
#include <i386/limits.h>
#include "benchmark.h"
#include "naive.h"
#include "track_back.h"
#include "track_back_optimized.h"

struct TestData0 {
  const int *array;
  size_t windowSize;
  size_t length;
};

struct Output0 {
  int *out;
  size_t length;
//  bool operator<(Output0 const& right) const
//  {
//    for (int i = 0; i < length; i++)
//      if (array1[i] == array2[i])
//  }
};


class SlidingWindowMaximumBenchmark : public Benchmark<TestData0, Output0> {

  int generateTestData(unsigned int seed, float t, TestData0 *testData) {
    srand(seed);
    const size_t size = (size_t const) (t * 200000 + 1);
    int *original = (int *) malloc(size * sizeof(int));
    for (size_t i = 0; i < size; i++)
      original[i] = rand() % INT_MAX;
    testData->array = original;
    testData->windowSize = size / 10;
    testData->length = size;
    return (int) size;
  };

  void prepareOutput(TestData0 *testData, Output0 *output) {
    output->out = (int *) calloc(testData->length, sizeof(int));
  }

public:

  SlidingWindowMaximumBenchmark() {
    addMethod("naive", [](const TestData0 &data, Output0 *output) {
        slidingWindowMaximum::naive(data.array, data.length, data.windowSize, output->out);
    });
    addMethod("trackBack", [](const TestData0 &data, Output0 *output) {
        slidingWindowMaximum::trackBack(data.array, data.length, data.windowSize, output->out);
    });

    addMethod("trackBackOptimized", [](const TestData0 &data, Output0 *output) {
        slidingWindowMaximum::trackBackOptimized(data.array, data.length, data.windowSize, output->out);
    });

  }
};

void benchmark1() {
  SlidingWindowMaximumBenchmark benchmark;
  benchmark.run(100, 4000);
}