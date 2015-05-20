// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#include "comparison_sort_in_place_benchmark.h"

#include "benchmark.h"
#include "bubble_sort.h"
#include "heap_sort.h"
#include "insertion_sort.h"
#include "quicksort.h"
#include "quicksort_3.h"
#include "quicksort_3_plus_shell.h"
#include "quicksort_3_threaded.h"
#include "quicksort_swap.h"
#include "quicksort_swap_plus_shell.h"
#include "quicksort_swap_plus_shell_threaded.h"
#include "quicksort_swap_threaded.h"
#include "shell_sort.h"

#include <sstream>

class TestData0 {

  int *_array;
  size_t _length;

public:

  const int *array() const {
    return _array;
  }

  size_t length() const {
    return _length;
  }

  TestData0(unsigned int seed, float size) {
    _length = (size_t const) size;
    seed += _length;
    srand(seed);
    _array = (int *) malloc(_length * sizeof(int));
    for (size_t i = 0; i < _length; i++) {
      int r = rand();
      srand((unsigned int) (r % _length));
      _array[i] = rand();
      srand((unsigned int) (r + seed + i));
    }

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

  size_t length() const {
    return _length;
  }

  Output0(const TestData0 *testData) {
    _length = testData->length();
    _out = (int *) malloc(testData->length() * sizeof(int));
    for (size_t i = 0; i < _length; i++)
      _out[i] = testData->array()[i];
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

int compare(const void *a, const void *b) {
  return *(int *) a - *(int *) b;
}

class ComparisonSortInPlaceBenchmark : public Benchmark<TestData0, Output0> {

protected:
  void add(const char *name,
      void (*method)(int *start, int *end)) {
    this->addMethod(name, [=](const TestData0 &data, Output0 *output) {
        method(output->out(), output->out() + output->length());
    });
  }

public:

  ComparisonSortInPlaceBenchmark() {

    this->addMethod("std::sort", [=](const TestData0 &data, Output0 *output) {
        std::sort(output->out(), output->out() + output->length());
    });

    this->addMethod("std::sort_heap", [=](const TestData0 &data, Output0 *output) {
        std::make_heap(output->out(), output->out() + output->length());
        std::sort_heap(output->out(), output->out() + output->length());
    });

    this->addMethod("<heapsort>", [=](const TestData0 &data, Output0 *output) {
        heapsort(output->out(), output->length(), sizeof(int), compare);
    });

    this->addMethod("<mergesort>", [=](const TestData0 &data, Output0 *output) {
        mergesort(output->out(), output->length(), sizeof(int), compare);
    });

    this->addMethod("std::qsort", [=](const TestData0 &data, Output0 *output) {
        std::qsort(output->out(), output->length(), sizeof(int), compare);
    });

    using namespace comparisonSortInPlace;

    add("bubbleSort", bubbleSort);

    add("insertionSort", insertionSort);
    add("shellSort", shellSort);

    add("heapSort", heapSort);

    add("quicksort", quicksort);
    add("quicksort3", quicksort3);
    add("quicksort3PlusShell", quicksort3PlusShell);
    add("quicksortSwap", quicksortSwap);
    add("quicksortSwapPlusShell", quicksortSwapPlusShell);

    this->addMethod("quicksortSwapThreaded", [=](const TestData0 &data, Output0 *output) {
        quicksortSwapThreaded(output->out(), output->out() + output->length(),
            std::max(5000, (int) (0.04 * output->length())));
    });

    this->addMethod("quicksortSwapPlusShellThreaded", [=](const TestData0 &data, Output0 *output) {
        quicksortSwapPlusShellThreaded(output->out(), output->out() + output->length(),
            std::max(5000, (int) (0.04 * output->length())));
    });

    this->addMethod("quicksort3Threaded", [=](const TestData0 &data, Output0 *output) {
        quicksort3Threaded(output->out(), output->out() + output->length(),
            std::max(5000, (int) (0.04 * output->length())));
    });

  }
};


void comparisonSortInPlaceBenchmark() {
  ComparisonSortInPlaceBenchmark benchmark;
  int samples = 75;
  int min = 100;
  int max = (int) 2e5;
  int distribution = 2;
  bool rounded = true;
  int cap = max * 125;
  benchmark.run(samples, min, max, distribution, rounded, cap, "Microseconds");
}