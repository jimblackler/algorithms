#include <stdlib.h>
#include <i386/limits.h>
#include "benchmark.h"
#include "naive.h"
#include "track_back.h"

struct TestData0 {
  const int *array;
  size_t windowSize;
  size_t length;
};

struct Output0 {
  int* out;
};

typedef void Signature(const int *in, size_t length, size_t windowSize, int *out);

class SlidingWindowMaximumBenchmark : public Benchmark<Signature, TestData0, Output0> {

  void generateTestData(unsigned int seed, TestData0* testData) {
    srand(seed);
    const size_t size = 20000000;
    int *original = (int *) malloc(size * sizeof(int));
    for (size_t i = 0; i < size; i++)
      original[i] = rand() % INT_MAX;
    testData->array = original;
    testData->windowSize = size / 10;
    testData->length = size;
  };

  void prepareOutput(TestData0* testData, Output0* output) {
    output->out = (int *) calloc(testData->length, sizeof(int));
  }

  void invoke(Signature &signature, const TestData0& data, Output0* output) {
    signature(data.array, data.length, data.windowSize, output->out);
  };

public:

  SlidingWindowMaximumBenchmark() {
    addMethod("naive", &slidingWindowMaximum::naive);
    addMethod("trackBack", &slidingWindowMaximum::trackBack);
  }
};

void benchmark1() {
  SlidingWindowMaximumBenchmark benchmark;
  benchmark.run();
}