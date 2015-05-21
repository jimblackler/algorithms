// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#include "comparison_sort_in_place_benchmark.h"

#include "../benchmark.h"

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
#include <stdlib.h>

template<typename T>
void generate(T &v) {
  v = (T) rand();
}

template<>
void generate<std::string>(std::string &v) {
  v = std::to_string(rand());
}

template<typename T>
int compare(const void *a, const void *b) {
  return *(T *) a - *(T *) b;
}

template<>
int compare<std::string>(const void *a, const void *b) {
  return ((std::string *) a)->compare(*((std::string *) b));
}

template<typename T>
class TestData0 {

  T *_array;
  size_t _length;

public:

  const T *array() const {
    return _array;
  }

  size_t length() const {
    return _length;
  }

  TestData0(unsigned int seed, float size) {
    _length = (size_t const) size;
    seed += _length;
    srand(seed);
    _array = new T[_length];
    for (size_t i = 0; i < _length; i++) {
      int r = rand();
      srand((unsigned int) (r % _length));
      generate(_array[i]);
      srand((unsigned int) (r + seed + i));
    }

  }

  ~TestData0() {
    delete[] _array;
  }

};

template<typename T>
class Output0 {
  T *_out;
  size_t _length;

public:
  T *out() const {
    return _out;
  }

  size_t length() const {
    return _length;
  }

  Output0(const TestData0<T> *testData) {
    _length = testData->length();
    _out = new T[_length];
    const T *array = testData->array();
    for (size_t i = 0; i < _length; i++)
      _out[i] = array[i];
  }

  ~Output0() {
    delete[] _out;
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

template<typename T>
class ComparisonSortInPlaceBenchmark : public Benchmark<TestData0<T>, Output0<T>> {

protected:
  void add(const char *name, void (*method)(T *start, T *end)) {
    this->addMethod(name, [=](const TestData0<T> &data, Output0<T> *output) {
        method(output->out(), output->out() + output->length());
    });
  }

public:

  ComparisonSortInPlaceBenchmark() {

    this->addMethod("std::sort", [](const TestData0<T> &data, Output0<T> *output) {
        T *array = output->out();
        std::sort(array, array + output->length());
    });

    this->addMethod("std::sort_heap", [](const TestData0<T> &data, Output0<T> *output) {
        std::make_heap(output->out(), output->out() + output->length());
        std::sort_heap(output->out(), output->out() + output->length());
    });

#ifdef __APPLE__
    this->addMethod("<heapsort>", [](const TestData0<T> &data, Output0<T> *output) {
        heapsort(output->out(), output->length(), sizeof(T), compare<T>);
    });

    this->addMethod("<mergesort>", [](const TestData0<T> &data, Output0<T> *output) {
        mergesort(output->out(), output->length(), sizeof(T), compare<T>);
    });
#endif

    this->addMethod("std::qsort", [](const TestData0<T> &data, Output0<T> *output) {
        std::qsort(output->out(), output->length(), sizeof(T), compare<T>);
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

    this->addMethod("quicksortSwapThreaded", [](const TestData0<T> &data, Output0<T> *output) {
        quicksortSwapThreaded(output->out(), output->out() + output->length(),
            std::max(5000, (int) (0.04 * output->length())));
    });

    this->addMethod("quicksortSwapPlusShellThreaded",
        [](const TestData0<T> &data, Output0<T> *output) {
            quicksortSwapPlusShellThreaded(output->out(), output->out() + output->length(),
                std::max(5000, (int) (0.04 * output->length())));
        });

    this->addMethod("quicksort3Threaded", [](const TestData0<T> &data, Output0<T> *output) {
        quicksort3Threaded(output->out(), output->out() + output->length(),
            std::max(5000, (int) (0.04 * output->length())));
    });

  }
};


void comparisonSortInPlaceBenchmark() {
  ComparisonSortInPlaceBenchmark<std::string> benchmark;
  int samples = 120;
  int min = 100;
  int max = (int) 1e6;
  int distribution = 2;
  bool rounded = true;
  int cap = max * 150;
  benchmark.run(samples, min, max, distribution, rounded, cap, "Microseconds");
}
