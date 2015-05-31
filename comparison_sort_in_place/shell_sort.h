// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#pragma once

#include "insertion_sort.h"

namespace comparisonSortInPlace {

template<typename T, typename F, typename I>
void insertionSortWithOffset(T start, T end, F less, I offset) {
  for (T fwd = start + offset; fwd < end; fwd++) {
    for (T rev = fwd; rev >= start + offset; rev -= offset) {
      if (less(*rev, rev[-offset]))
        std::swap(*rev, rev[-offset]);
      else
        break;
    }
  }
}

template<typename T, typename F>
void shellSort(T start, T end, F less) {
  auto length = end - start;

  int divide = 9;
  for (auto offset = length / divide; offset > 1; offset /= divide) {
    insertionSortWithOffset(start, end, less, offset);
  }
  insertionSort(start, end, less);
}
}