// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#pragma once

#include <type_traits>
#include <algorithm>

namespace comparisonSortInPlace {

template<typename T, typename F>
void insertionSort(T *start, T *end, F less) {

  for (T *fwd = start + 1; fwd < end; fwd++) {
    T value = std::move(*fwd);
    T *rev;
    for (rev = fwd; rev > start && less(value, rev[-1]); rev -= 1) {
      *rev = std::move(rev[-1]);
    }
    *rev = value;
  }
}

}