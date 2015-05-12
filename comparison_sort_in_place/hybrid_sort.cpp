// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#include <algorithm>
#include "hybrid_sort.h"
#include "shell_sort.h"

namespace comparisonSortInPlace {

void hybridSort(int *start, int *end) {
  if (end - start <= 2000)
    return shellSort(start, end);
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
  hybridSort(start, a);  // Sort <pivot region
  hybridSort(b, end);  // Sort >pivot region
}
}