// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#include "heap_sort.h"

namespace comparisonSortInPlace {

static inline void siftDown(int position, int value, int *start, int count) {

  while (true) {
    int childPosition = 2 * position + 1;
    if (childPosition >= count)
      break;  // No children.

    int childValue = start[childPosition];

    int rightChildPosition = childPosition + 1;
    // Right child exists?
    if (rightChildPosition < count) {
      int rightChildValue = start[rightChildPosition];
      // And is greater than left?
      if (rightChildValue > childValue) {
        // Select this child.
        childPosition = rightChildPosition;
        childValue = rightChildValue;
      }
    }

    if (value >= childValue)
      break;  // Heap constraint met.

    // Promote child to parent, then repeat.
    start[position] = childValue;
    position = childPosition;
  }
  start[position] = value;
}

void heapSort(int *start, int *end) {
  int count = (int) (end - start);

  for (int k = (count -  2) / 2; k >= 0; k--) {
    siftDown(k, start[k], start, count);
  }

  while (count--) {
    int maximal = start[0];
    int replaced = start[count];
    start[count] = maximal;

    siftDown(0, replaced, start, count);
  }

}
}