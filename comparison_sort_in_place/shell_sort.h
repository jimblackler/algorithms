// (c) Jim Blackler (jimblacker@gmail.com)
// Free software under GNU General Public License Version 2 (see LICENSE).

#pragma once

#include "insertion_sort.h"

namespace comparisonSortInPlace {

template<typename T, typename F, typename I>
void insertionSortWithOffset(T *start, T *end, F less, I offset) {
  for (T *fwd = start + offset; fwd < end; fwd++) {
    T value = std::move(*fwd);
    T *rev;
    for (rev = fwd; rev >= start + offset; rev -= offset) {
      if (!less(value, rev[-offset]))
        break;
      *rev = std::move(rev[-offset]);
    }
    *rev = value;
  }
}

template <typename T, typename F>
void shellSort(T *start, T *end, F less) {
  auto length = end - start;
  if (length <= 2) {
    if (length == 2 && less(start[1], *start))
      std::swap(*start, start[1]);
    return;
  }
  int divide = 9;
  for (auto offset = length / divide; offset > 1; offset /= divide) {
    insertionSortWithOffset(start, end, less, (int) offset);
  }
  insertionSort(start, end, less);
}
}