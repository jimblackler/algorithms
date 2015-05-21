// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#include <stddef.h>

namespace comparisonSortInPlace {

template <typename T>
void bubbleSort(T *start, T *end) {
  for (T *endPtr = end; end >= start; end--) {
    for (T *ptr = start; &ptr[1] < endPtr; ptr++) {
      if (*ptr > ptr[1])
        std::swap(*ptr, ptr[1]);
    }
  }
}

}