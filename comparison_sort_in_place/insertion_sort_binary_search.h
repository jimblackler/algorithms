// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#pragma once

#include <type_traits>
#include <algorithm>

namespace comparisonSortInPlace {

template<typename T, typename F>
void insertionSortBinarySearch(T *start, T *end, F less) {
  for (T *fwd = start + 1; fwd < end; fwd++) {
    T value = std::move(*fwd);

    T *max = fwd;
    T *min = start;
    while (min != max) {
      T *ptr = min + (max - min) / 2;
      if (less(*ptr, value))
        min = ptr + 1;
      else
        max = ptr;
    }
    std::move_backward(min, fwd, fwd + 1);
    *min = value;
  }
}

}