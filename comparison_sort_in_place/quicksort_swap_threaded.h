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

  T pivot = start[length / 2];
  T *lt;
  T *gt = end;
  for (lt = start; lt < gt; lt++) {
    if (less(*lt, pivot))
      continue;
    do {
      gt--;
      if (gt == lt)
        goto escape;
    } while (!less(*gt, pivot));
    std::swap(*lt, *gt);
  }

  escape:;
  if (start == lt) {
    for (T *ptr = start; ptr < end; ptr++) {
      if (!less(pivot, *ptr))
        std::swap(*ptr, *lt++);
    }
    quicksortSwapThreaded(lt, end, less, minSize);
  } else {
    std::thread t1([=]() {
        quicksortSwapThreaded(start, lt, less, minSize);
    });
    quicksortSwapThreaded(lt, end, less, minSize);
    t1.join();
  }
}

}