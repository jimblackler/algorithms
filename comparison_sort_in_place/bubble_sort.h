// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#include <utility>

namespace comparisonSortInPlace {

template <typename T, typename F>
void bubbleSort(T start, T end, F less) {
  for (T endPtr = end; end >= start; end--) {
    for (T ptr = start; ptr + 1 < endPtr; ptr++) {
      if (less(ptr[1], *ptr))
        std::swap(*ptr, ptr[1]);
    }
  }
}

}