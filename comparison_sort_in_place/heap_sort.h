// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#include "stddef.h"

namespace comparisonSortInPlace {

template<typename T>
void siftDown(T position, T value, T *start, T count) {

  while (true) {
    T childPosition = 2 * position + 1;
    if (childPosition >= count)
      break;  // No children.

    T childValue = start[childPosition];

    T rightChildPosition = childPosition + 1;
    // Right child exists?
    if (rightChildPosition < count) {
      T rightChildValue = start[rightChildPosition];
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

template<typename T>
void heapSort(T *start, T *end) {
  T count = (T) (end - start);

  for (T k = (count - 2) / 2; k >= 0; k--) {
    siftDown(k, start[k], start, count);
  }

  while (count--) {
    T maximal = start[0];
    T replaced = start[count];
    start[count] = maximal;

    siftDown(0, replaced, start, count);
  }
}

}