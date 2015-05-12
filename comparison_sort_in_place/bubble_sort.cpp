// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#include "bubble_sort.h"

#include <deque>

namespace comparisonSortInPlace {

void bubbleSort(int *start, int *end) {
  for (int *endPtr = end; end >= start; end--) {
    for (int *ptr = start; &ptr[1] < endPtr; ptr++) {
      if (*ptr > ptr[1])
        std::swap(*ptr, ptr[1]);
    }
  }
}
}