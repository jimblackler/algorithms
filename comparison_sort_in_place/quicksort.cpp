// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#include "quicksort.h"
#include "insertion_sort.h"

#include <deque>

namespace comparisonSortInPlace {

void quicksort(int *start, int *end) {
  int pivot = *(start + (end - start) / 2);
  int *divider = start;
  for (int *ptr = start; ptr < end; ptr++) {
    if (*ptr < pivot)
      std::swap(*ptr, *divider++);
  }
  if (start == divider || divider == end)
    return insertionSort(start, end);

  quicksort(start, divider);
  quicksort(divider, end);
}
}