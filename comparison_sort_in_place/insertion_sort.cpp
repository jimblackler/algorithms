// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#include "bubble_sort.h"

namespace comparisonSortInPlace {

void insertionSort(int *start, int *end) {
  for (int *fwd = start + 1; fwd < end; fwd++) {
    int value = *fwd;
    int *rev;
    for (rev = fwd; rev > start; rev--) {
      int v = rev[-1];
      if (v <= value)
        break;
      *rev = v;
    }
    *rev = value;
  }
}
}