// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#include <thread>
#include "quicksort_swap.h"
#include "quicksort_swap_plus_insertion.h"

namespace comparisonSortInPlace {

template<typename T, typename Predicate, typename Size>
void quicksortSwapPlusInsertionThreaded(T start, T end, Predicate less, Size minSize) {
  auto length = end - start;
  if (length <= minSize)
    return quicksortSwapPlusInsertion(start, end, less);

  T pivot = end - 1;
  T c = start + length / 2;
  if (length < 14)
    std::swap(*c, *pivot);
  else if (length < 79)
    std::swap(*median3(less, c - 1), *pivot);
  else if (length < 227)
    std::swap(*median5(less, c - 2), *pivot);
  else if (length < 415)
    std::swap(*medianByPartialSort(c - 3, c + 4, less), *pivot);
  else
    std::swap(*medianByPartialSort(c - 4, c + 5, less), *pivot);

  T ge = pivot;

  for (T lt = start; lt < ge; lt++) {
    if (!less(*lt, *pivot)) {
      while (--ge != lt) {
        if (less(*ge, *pivot)) {
          std::swap(*lt, *ge);
          break;
        }
      }
    }
  }

  if (start == ge) {
    for (T ptr = start; ptr < end; ptr++) {
      if (!less(*pivot, *ptr)) {
        std::swap(*ptr, *ge++);
      }
    }
    quicksortSwapPlusInsertionThreaded(ge, end, less, minSize);
  } else {

    std::thread t1([=]() {
        quicksortSwapPlusInsertionThreaded(start, ge, less, minSize);
    });
    std::swap(*pivot, *ge++);
    quicksortSwapPlusInsertionThreaded(ge, end, less, minSize);
    t1.join();
  }
}

}