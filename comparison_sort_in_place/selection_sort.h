// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#pragma once

#include <type_traits>
#include <algorithm>

namespace comparisonSortInPlace {

template<typename T, typename F>
void selectionSort(T start, T end, F less) {
  for (T a = start; a < end; a++) {
    T smallest = a;
    for (T b = a + 1; b < end; b++) {
      if (less(*b, *smallest))
        smallest = b;
    }
    std::swap(*a, *smallest);
  }
}

}