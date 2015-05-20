// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#include "quicksort_swap_plus_shell.h"

#include "shell_sort.h"

namespace comparisonSortInPlace {

void quicksortSwapPlusShell(int *start, int *end) {
  auto length = end - start;
  if (length <= 250)
    return shellSort(start, end);

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
  } else {
    quicksortSwapPlusShell(start, lt);
  }
  quicksortSwapPlusShell(lt, end);
}
}