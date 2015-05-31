// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#pragma once

#include <type_traits>
#include <algorithm>

namespace comparisonSortInPlace {

template<typename T, typename F>
void insertionSort(T start, T end, F less) {
  for (T fwd = start + 1; fwd < end; fwd++) {
    for (T rev = fwd; rev > start && less(*rev, rev[-1]); rev -= 1) {
      std::swap(*rev, rev[-1]);
    }
  }
}

}