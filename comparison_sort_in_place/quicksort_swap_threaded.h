// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#include <thread>
#include <stddef.h>
#include "quicksort_swap.h"

namespace comparisonSortInPlace {

template<typename T>
void quicksortSwapThreaded(T *start, T *end, T minSize) {
  auto length = end - start;
  if (length <= minSize)
    return quicksortSwap(start, end);

  T pivot = start[length / 2];
  T *lt = start;
  T *gt = end;

  while (lt < gt) {
    T a = *lt;
    if (a >= pivot) {
      T b;
      do {
        gt--;
        if (gt == lt)
          goto escape;
        b = *gt;
      } while (b >= pivot);

      *lt = b;
      *gt = a;
    }
    lt++;
  }

  escape:;
  if (start == lt) {
    for (T *ptr = start; ptr < end; ptr++) {
      if (*ptr != pivot)
        continue;
      *ptr = *lt;
      *lt++ = pivot;
    }
    quicksortSwapThreaded(lt, end, minSize);  // Sort <pivot region
  } else {
    std::thread t1([=]() {
        quicksortSwapThreaded(start, lt, minSize);  // Sort <pivot region
    });
    quicksortSwapThreaded(lt, end, minSize);
    t1.join();
  }
}

}