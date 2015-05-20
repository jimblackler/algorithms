// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#include "quicksort_3.h"

namespace comparisonSortInPlace {

void quicksort3(int *start, int *end) {
  auto length = end - start;
  if (length <= 1)
    return;
  int pivot = start[length / 2];
  int *a = start;  // [start, a) is <pivot region
  int *b = start;  // [b, ptr) is >pivot region
  for (int *ptr = start; ptr < end; ptr++) {
    int v = *ptr;
    if (v > pivot)
      continue;
    *ptr = *b++;  // >pivot region shifted
    if (v != pivot)
      *a++ = v;  // <pivot region extended
  }
  for (int *ptr = a; ptr < b; ptr++)
    *ptr = pivot;  // Fill =pivot region
  quicksort3(start, a);  // Sort <pivot region
  quicksort3(b, end);  // Sort >pivot region
}
}