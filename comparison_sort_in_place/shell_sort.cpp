// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#include "shell_sort.h"

namespace comparisonSortInPlace {

static inline void insertionSortWithOffset(int *start, int *end, int offset) {
  for (int *fwd = start + offset; fwd < end; fwd++) {
    int value = *fwd;
    int *rev;
    for (rev = fwd; rev >= start + offset; rev -= offset) {
      int v = rev[-offset];
      if (v <= value)
        break;
      *rev = v;
    }
    *rev = value;
  }
}

void shellSort(int *start, int *end) {
  auto length = end - start;
  int divide = 9;
  for (auto offset = length / divide; offset > 1; offset /= divide) {
    insertionSortWithOffset(start, end, (int) offset);
  }
  insertionSortWithOffset(start, end, 1);
}
}