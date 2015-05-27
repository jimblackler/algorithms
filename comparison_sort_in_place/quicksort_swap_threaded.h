// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#include <thread>
#include "quicksort_swap.h"

namespace comparisonSortInPlace {

template<typename T, typename F>
void quicksortSwapThreaded(T *start, T *end, F less, int minSize) {
  auto length = end - start;
  if (length <= minSize)
    return quicksortSwap(start, end, less);

  T *lt;
  T *pivot = end - 1;
  std::swap(start[length / 2], *pivot);
  T *ge = pivot;
  for (lt = start; lt < ge; lt++) {
    if (less(*lt, *pivot))
      continue;
    do {
      ge--;
      if (ge == lt)
        goto escape;
    } while (!less(*ge, *pivot));
    std::swap(*lt, *ge);
  }

  escape:;
  if (start == lt) {
    for (T *ptr = start; ptr < end; ptr++) {
      if (!less(*pivot, *ptr)) {
        std::swap(*ptr, *lt++);
      }
    }
    quicksortSwapThreaded(lt, end, less, minSize);
  } else {

    std::thread t1([=]() {
        quicksortSwapThreaded(start, lt, less, minSize);
    });
    std::swap(*pivot, *lt++);
    quicksortSwapThreaded(lt, end, less, minSize);
    t1.join();
  }
}

}