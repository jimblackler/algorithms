// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#include "stddef.h"

namespace comparisonSortInPlace {

template<typename T>
void insertionSort(T *start, T *end) {
  for (T *fwd = start + 1; fwd < end; fwd++) {
    T value = *fwd;
    T *rev;
    for (rev = fwd; rev > start; rev--) {
      T v = rev[-1];
      if (v <= value)
        break;
      *rev = v;
    }
    *rev = value;
  }
}

}