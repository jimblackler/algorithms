// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#include "quicksort.h"

namespace comparisonSortInPlace {

void quicksort(int *start, int *end) {
  auto length = end - start;
  if (length <= 1)
    return;
  int pivot = start[length / 2];
  int *divider = start;
  for (int *ptr = start; ptr < end; ptr++) {
    int v = *ptr;
    if (v >= pivot)
      continue;
    *ptr = *divider;
    *divider++ = v;
  }

  if (start == divider) {
    for (int *ptr = start; ptr < end; ptr++) {
      if (*ptr != pivot)
        continue;
      *ptr = *divider;
      *divider++ = pivot;
    }
  } else {
    quicksort(start, divider);
  }

  quicksort(divider, end);
}
}