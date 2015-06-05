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

template<typename T>
T generate() {
  return (T) rand();
}

template<>
std::string generate<std::string>() {
  return std::to_string(rand());
}

template<typename T>
using LessType = std::function<bool(const T &, const T &)>;

template<typename T>
bool less(const T &a, const T &b) {
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
  std::vector<T> _out;
  size_t _length;
public:

  T *out() {
    return &_out[0];
  }

  const T *out() const {
    return &_out[0];
  }

  size_t length() const {
    return _length;
  }

  Output0(const TestData0<T> *testData) {
    _length = testData->length();
    _out = testData->array();
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
class Output1 : public Output0<T> {
private:
  int compareCount = 0;

public:
  Output1(TestData0<T> const *testData) : Output0<T>(testData) {

  }

  int getCompareCount() const {
    return compareCount;
  }

  void setCompareCount(int i) {
    compareCount = i;
  }
};

template<typename T, typename _Output>
class BaseSortBenchmark : public Benchmark<TestData0<T>, _Output> {

public:
  std::string isValid(const TestData0<T> &testData, const _Output &output) const {
    auto length = output.length();

    if (testData.length() != length) {
      return "Lengths are different";
    }
    auto array = output.out();

    for (int i = 1; i != length; i++) {
      if (less<T>(array[i], array[i - 1])) {
        return "Element found out of order";
      }
    }
    return std::string();
  }
};

template<typename T>
class ComparisonSortInPlaceBenchmark : public BaseSortBenchmark<T, Output0<T>> {
public:

  using I = T *;

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

    this->addMethod("bubbleSort", [](const TestData0<T> &data, Output0<T> *output) {
        bubbleSort(output->out(), output->out() + output->length(), less<T>);
    });

    this->addMethod("insertionSort", [](const TestData0<T> &data, Output0<T> *output) {
        insertionSort(output->out(), output->out() + output->length(), less<T>);
    });

    this->addMethod("insertionSortBinarySearch",
        [](const TestData0<T> &data, Output0<T> *output) {
            insertionSortBinarySearch(output->out(), output->out() + output->length(), less<T>);
        });

    this->addMethod("shellSort",
        [](const TestData0<T> &data, Output0<T> *output) {
            shellSort(output->out(), output->out() + output->length(), less<T>);
        });

    this->addMethod("heapSort",
        [](const TestData0<T> &data, Output0<T> *output) {
            heapSort(output->out(), output->out() + output->length(), less<T>);
        });

    this->addMethod("quicksort",
        [](const TestData0<T> &data, Output0<T> *output) {
            quicksort(output->out(), output->out() + output->length(), less<T>,
                2, [](I start, I end) {
                    maxTwo(start, end, less<T>);
                });
        });

    this->addMethod("quicksort3",
        [](const TestData0<T> &data, Output0<T> *output) {
            quicksort3(output->out(), output->out() + output->length(), less<T>,
                2, [](I start, I end) {
                    maxTwo(start, end, less<T>);
                });;
        });

    this->addMethod("quicksortSwap",
        [](const TestData0<T> &data, Output0<T> *output) {
            quicksortSwap(output->out(), output->out() + output->length(), less<T>,
                2, [](I start, I end) {
                    maxTwo(start, end, less<T>);
                });
        });

    this->addMethod("quicksortSwapPlusInsertion@10",
        [](const TestData0<T> &data, Output0<T> *output) {
            quicksortSwap(output->out(), output->out() + output->length(), less<T>,
                10, [](I start, I end) {
                    insertionSort(start, end, less<T>);
                });
        });

    this->addMethod("quicksortSwapPlusShell@160",
        [](const TestData0<T> &data, Output0<T> *output) {
            quicksortSwap(output->out(), output->out() + output->length(), less<T>,
                160, [](I start, I end) {
                    shellSort(start, end, less<T>);
                });
        });

    this->addMethod("quicksortSwapThreadedPlusInsertion@10",
        [](const TestData0<T> &data, Output0<T> *output) {
            quicksortSwapThreaded(output->out(), output->out() + output->length(), less<T>,
                std::max(5000, (int) (0.04 * output->length())),
                10, [](I start, I end) {
                    insertionSort(start, end, less<T>);
                });
        });

    this->addMethod("quicksortSwapThreadedShell@160",
        [](const TestData0<T> &data, Output0<T> *output) {
            quicksortSwapThreaded(output->out(), output->out() + output->length(), less<T>,
                std::max(5000, (int) (0.04 * output->length())),
                160, [](I start, I end) {
                    shellSort(start, end, less<T>);
                });
        });
  }

};

template<typename T>
class ComparisonSortInPlaceComparisonCount : public BaseSortBenchmark<T, Output1<T>> {

  template <typename M>
  void addMethod0(char const *name, M invoker) {

    this->addMethod(name, [&](const TestData0<T> &data, Output1<T> *output) {
        int compareCount = 0;
        auto ls = [&](const T &a, const T &b) {
            compareCount++;
            return less<T>(a, b);
        };
        invoker(data, output, ls);
        output->setCompareCount(compareCount);
    });
  }


public:

  using I = T *;

  ComparisonSortInPlaceComparisonCount() {
    this->addMethod0("std::sort",
        [](const TestData0<T> &data, Output0<T> *output, LessType<T> ls) {
            std::sort(output->out(), output->out() + output->length(), ls);
        });

    using namespace comparisonSortInPlace;


    this->addMethod0("bubbleSort",
        [](const TestData0<T> &data, Output0<T> *output, LessType<T> ls) {
            bubbleSort(output->out(), output->out() + output->length(), ls);
        });

    this->addMethod0("insertionSort",
        [](const TestData0<T> &data, Output0<T> *output, LessType<T> ls) {
            insertionSort(output->out(), output->out() + output->length(), ls);
        });

    this->addMethod0("insertionSortBinarySearch",
        [](const TestData0<T> &data, Output0<T> *output, LessType<T> ls) {
            insertionSortBinarySearch(output->out(), output->out() + output->length(), ls);
        });

    this->addMethod0("heapSort",
        [](const TestData0<T> &data, Output0<T> *output, LessType<T> ls) {
            heapSort(output->out(), output->out() + output->length(), ls);
        });

    this->addMethod0("quicksort",
        [](const TestData0<T> &data, Output0<T> *output, LessType<T> ls) {
            quicksort(output->out(), output->out() + output->length(), ls,
                2, [&](I start, I end) {
                    maxTwo(start, end, ls);
                });
        });

    this->addMethod0("quicksort3",
        [](const TestData0<T> &data, Output0<T> *output, LessType<T> ls) {
            quicksort3(output->out(), output->out() + output->length(), ls,
                2, [&](I start, I end) {
                    maxTwo(start, end, ls);
                });
        });

    this->addMethod0("quicksortSwap",
        [](const TestData0<T> &data, Output0<T> *output, LessType<T> ls) {
            quicksortSwap(output->out(), output->out() + output->length(), ls,
                2, [&](I start, I end) {
                    maxTwo(start, end, ls);
                });
        });

    this->addMethod0("quicksortSwapPlusShell@160",
        [](const TestData0<T> &data, Output0<T> *output, LessType<T> ls) {
            quicksortSwap(output->out(), output->out() + output->length(), ls,
                160, [&](I start, I end) {
                    shellSort(start, end, ls);
                });
        });
    
    this->addMethod0("quicksortSwapPlusInsertion@10",
        [](const TestData0<T> &data, Output0<T> *output, LessType<T> ls) {
            quicksortSwap(output->out(), output->out() + output->length(), ls,
                10, [&](I start, I end) {
                    insertionSort(start, end, ls);
                });
        });
  }


  long long yAxisValue(long long microseconds, const Output1<T> &output) {
    return output.getCompareCount();
  }
};


void comparisonSortInPlaceBenchmark() {
  if (false) {
    ComparisonSortInPlaceBenchmark<int> benchmark;
    int samples = 50;
    int min = 5;
    long max = (long) 5e6;
    int distribution = 3;
    bool rounded = true;
    long cap = max * 500;
    benchmark.run(samples, min, max, distribution, rounded, cap, "Microseconds");
  } else {
    ComparisonSortInPlaceComparisonCount<int> benchmark;
    int samples = 50;
    int min = 5;
    long max = (long) 1e6;
    int distribution = 3;
    bool rounded = true;
    long cap = max * 25;
    benchmark.run(samples, min, max, distribution, rounded, cap, "Compares");
  }
}

