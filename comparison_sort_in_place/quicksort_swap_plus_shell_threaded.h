// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#include "quicksort_swap_plus_shell.h"

#include <thread>
#include <stddef.h>

namespace comparisonSortInPlace {

template <typename T>
void quicksortSwapPlusShellThreaded(T *start, T *end, T minSize) {
  auto length = end - start;
  if (length <= minSize)
    return quicksortSwapPlusShell(start, end);

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