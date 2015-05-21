// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

namespace comparisonSortInPlace {

template<typename I, typename T>
void siftDown(I position, T value, T *start, I count) {

  while (true) {
    I childPosition = 2 * position + 1;
    if (childPosition >= count)
      break;  // No children.

    T childValue = start[childPosition];

    I rightChildPosition = childPosition + 1;
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
  auto count = end - start;

  for (auto k = (count - 2) / 2; k >= 0; k--) {
    siftDown(k, start[k], start, count);
  }

  while (count--) {
    T maximal = start[0];
    T replaced = start[count];
    start[count] = maximal;

    siftDown(0, replaced, start, (int) count);
  }
}

}