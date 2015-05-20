// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#include "quicksort_swap_threaded.h"

#include "quicksort_swap.h"

#include <thread>

namespace comparisonSortInPlace {

void quicksortSwapThreaded(int *start, int *end, int minSize) {
  auto length = end - start;
  if (length <= minSize)
    return quicksortSwap(start, end);

  int pivot = start[length / 2];
  int *lt = start;
  int *gt = end;

  while (lt < gt) {
    int a = *lt;
    if (a >= pivot) {
      int b;
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
    for (int *ptr = start; ptr < end; ptr++) {
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