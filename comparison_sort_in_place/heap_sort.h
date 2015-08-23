// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

namespace comparisonSortInPlace {

template<typename F, typename T>
void siftDown(T position, T start, T end, F less) {

  while (true) {
    T childPosition = start + 2 * (position - start) + 1;
    if (childPosition >= end)
      break;  // No children.

    T rightChildPosition = childPosition + 1;
    // Right child exists?
    if (rightChildPosition < end) {
      // And is greater than left?
      if (less(*childPosition, *rightChildPosition)) {
        // Select this child.
        childPosition = rightChildPosition;
      }
    }

    if (less(*childPosition, *position))
      break;  // Heap constraint met.

    // Promote child to parent, then repeat.
    std::swap(*position, *childPosition);
    position = childPosition;
  }
}

template<typename T, typename F>
void heapSort(T start, T end, F less) {
  auto count = end - start;

  for (auto k = (count - 2) / 2; k >= 0; k--) {
    siftDown(start + k, start, end, less);
  }

  while (count--) {
    std::swap(start[0], start[count]);
    siftDown(start, start, start + count, less);
  }
}

}