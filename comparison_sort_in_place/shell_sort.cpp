// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#include <algorithm>
#include "shell_sort.h"

namespace comparisonSortInPlace {

static void insertionSortWithOffset(int *start, int *end, int offset) {
  for (int *fwd = start + offset; fwd < end; fwd += offset) {
    int value = *fwd;
    int *rev;
    for (rev = fwd; rev > start; rev -= offset) {
      int v = rev[-offset];
      if (v <= value)
        break;
      *rev = v;
    }
    *rev = value;
  }
}

void shellSort(int *start, int *end) {
  float decline = 0.175f;
  for (unsigned long offset = (unsigned long) (decline * (end - start));
       offset > 1; offset *= decline) {
    for (int s = 0; s < offset; s++) {
      insertionSortWithOffset(start + s, end, (int) offset);
    }
  }
  insertionSortWithOffset(start, end, 1);

}
}