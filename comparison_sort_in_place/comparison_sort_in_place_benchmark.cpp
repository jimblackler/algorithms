// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#include "comparison_sort_in_place_benchmark.h"

#include "../benchmark.h"

#include "bubble_sort.h"
#include "heap_sort.h"
#include "insertion_sort.h"
#include "insertion_sort_binary_search.h"
#include "max_two.h"
#include "quicksort.h"
#include "quicksort_3.h"
#include "quicksort_swap.h"
#include "quicksort_swap_threaded.h"
#include "shell_sort.h"

#include <sstream>
#include <stdlib.h>

template<typename T>
T generate() {
  return (T) rand();
}

template<>
std::string generate<std::string>() {
  return std::to_string(rand());
}

template<typename T>
auto less = [](const T &a, const T &b) {
    return a < b;
};

template<typename T>
int compare(const void *a, const void *b) {
  T *_a = (T *) a;
  T *_b = (T *) b;
  if (less<T>(*_a, *_b))
    return -1;
  if (less<T>(*_b, *_a))
    return 1;
  return 0;
}

template<typename T>
class TestData0 {

  std::vector<T> _array;
  size_t _length;

public:

  const std::vector<T> &array() const {
    return _array;
  }

  size_t length() const {
    return _length;
  }

  TestData0(unsigned int seed, float size) {
    _length = (size_t const) size;
    seed += _length;
    srand(seed);

    for (size_t i = 0; i < _length; i++) {
      auto r = rand();
      double t = pow((float) r / RAND_MAX, 2);

      srand((unsigned int) (t * _length));
      _array.push_back(generate<T>());
      srand((unsigned int) (r + seed + i));
    }
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
    auto array = testData->array();
    for (size_t i = 0; i < _length; i++) {
      _out[i] = array[i];
    }
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
      if (less<T>(_out[i], right._out[i])) {
        return true;
      } else if (less<T>(right._out[i], _out[i])) {
        return false;
      }
    }
    return false;
  }
};

template<typename T>
class ComparisonSortInPlaceBenchmark : public Benchmark<TestData0<T>, Output0<T>> {

public:
  std::string isValid(const TestData0<T> &testData, const Output0<T> &output) const {
    auto length = output.length();

    if (testData.length() != length) {
      return "Lengths are different";
    }
    auto *array = output.out();

    for (int i = 1; i != length; i++) {
      if (less<T>(array[i], array[i - 1])) {
        return "Element found out of order";
      }
    }
    return std::string();
  }

  ComparisonSortInPlaceBenchmark() {

    this->addMethod("std::sort", [](const TestData0<T> &data, Output0<T> *output) {
        std::sort(output->out(), output->out() + output->length(), less<T>);
    });

    this->addMethod("std::sort_heap", [](const TestData0<T> &data, Output0<T> *output) {
        std::make_heap(output->out(), output->out() + output->length(), less<T>);
        std::sort_heap(output->out(), output->out() + output->length(), less<T>);
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

    this->addMethod("bubbleSort", [=](const TestData0<T> &data, Output0<T> *output) {
        bubbleSort(output->out(), output->out() + output->length(), less<T>);
    });

    this->addMethod("insertionSort", [=](const TestData0<T> &data, Output0<T> *output) {
        insertionSort(output->out(), output->out() + output->length(), less<T>);
    });

    this->addMethod("insertionSortBinarySearch",
        [=](const TestData0<T> &data, Output0<T> *output) {
            insertionSortBinarySearch(output->out(), output->out() + output->length(), less<T>);
        });

    this->addMethod("shellSort", [=](const TestData0<T> &data, Output0<T> *output) {
        shellSort(output->out(), output->out() + output->length(), less<T>);
    });

    this->addMethod("heapSort", [=](const TestData0<T> &data, Output0<T> *output) {
        heapSort(output->out(), output->out() + output->length(), less<T>);
    });

    this->addMethod("quicksort", [=](const TestData0<T> &data, Output0<T> *output) {
        quicksort(output->out(), output->out() + output->length(), less<T>,
            2, [=](T *start, T *end) {
                max_two(start, end, less<T>);
            });
    });

    this->addMethod("quicksort3", [=](const TestData0<T> &data, Output0<T> *output) {
        quicksort3(output->out(), output->out() + output->length(), less<T>,
            2, [=](T *start, T *end) {
                max_two(start, end, less<T>);
            });;
    });

    this->addMethod("quicksortPlusShell@160", [=](const TestData0<T> &data, Output0<T> *output) {
        quicksort(output->out(), output->out() + output->length(), less<T>,
            160, [=](T *start, T *end) {
                shellSort(start, end, less<T>);
            });
    });

    this->addMethod("quicksortSwapPlusShell@160", [=](const TestData0<T> &data, Output0<T> *output) {
        quicksortSwap(output->out(), output->out() + output->length(), less<T>,
            160, [=](T *start, T *end) {
                shellSort(start, end, less<T>);
            });
    });

    this->addMethod("quicksortSwap", [=](const TestData0<T> &data, Output0<T> *output) {
        quicksortSwap(output->out(), output->out() + output->length(), less<T>,
            2, [=](T *start, T *end) {
                max_two(start, end, less<T>);
            });
    });

    this->addMethod("quicksortSwapThreaded", [](const TestData0<T> &data, Output0<T> *output) {
        quicksortSwapThreaded(output->out(), output->out() + output->length(), less<T>,
            std::max(5000, (int) (0.04 * output->length())),
            2, [=](T *start, T *end) {
                max_two(start, end, less<T>);
            });
    });

    this->addMethod("quicksortSwapThreadedShell@160", [](const TestData0<T> &data, Output0<T> *output) {
        quicksortSwapThreaded(output->out(), output->out() + output->length(), less<T>,
            std::max(5000, (int) (0.04 * output->length())),
            160, [=](T *start, T *end) {
                shellSort(start, end, less<T>);
            });
    });

  }
};

void comparisonSortInPlaceBenchmark() {
  ComparisonSortInPlaceBenchmark<float> benchmark;
  int samples = 100;
  int min = 1;
  long max = (long) 1e6;
  int distribution = 2;
  bool rounded = true;
  long cap = max * 500;
  benchmark.run(samples, min, max, distribution, rounded, cap, "Microseconds");
}
