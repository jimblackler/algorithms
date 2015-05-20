// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#include "quicksort_swap_plus_shell_threaded.h"

#include "quicksort_swap_plus_shell.h"

#include <thread>

namespace comparisonSortInPlace {

void quicksortSwapPlusShellThreaded(int *start, int *end, int minSize) {
  auto length = end - start;
  if (length <= minSize)
    return quicksortSwapPlusShell(start, end);

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
    quicksortSwapPlusShellThreaded(lt, end, minSize);
  } else {
    std::thread t1([=]() {
        quicksortSwapPlusShellThreaded(start, lt, minSize);
    });
    quicksortSwapPlusShellThreaded(lt, end, minSize);
    t1.join();
  }
}
}
