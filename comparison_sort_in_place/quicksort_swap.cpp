// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#include "quicksort.h"

namespace comparisonSortInPlace {

void quicksortSwap(int *start, int *end) {
  auto length = end - start;
  if (length <= 1)
    return;

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
      if (*ptr == pivot) {
        *ptr = *lt;
        *lt++ = pivot;
      }
    }
  } else {
    quicksortSwap(start, lt);
  }
  quicksortSwap(lt, end);
}
}