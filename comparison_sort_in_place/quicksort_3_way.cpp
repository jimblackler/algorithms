// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#include "quicksort_3_way.h"
#include "insertion_sort.h"

#include <deque>

namespace comparisonSortInPlace {

void quicksort3Way(int *start, int *end) {
  if (end - start <= 1)
    return;
  int pivot = *(start + (end - start) / 2);
  int *a = start;  // [start, a) is <pivot region
  int *b = start;  // [b, ptr) is >pivot region
  for (int *ptr = start; ptr < end; ptr++) {
    int v = *ptr;
    if (v > pivot)
      continue;
    *ptr = *b++;  // >pivot region shifted
    if (v < pivot)
      *a++ = v;  // <pivot region extended
  }
  for (int *ptr = a; ptr < b; ptr++)
    *ptr = pivot;  // Fill =pivot region
  quicksort3Way(start, a);  // Sort <pivot region
  quicksort3Way(b, end);  // Sort >pivot region
}
}